#ifdef _WIN64

// INFO private X64 implementation
#include <HookEngine/private/x64/HookEngine64.h>
#include <HookEngine/private/x64/CodeCaveFinder64.h>

#include <udis86/udis86.h>

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace HookEngine {

  unsigned char templateProlog[] = {
    0x48, 0x8B, 0x04, 0x24,
    0x48, 0x89, 0x44, 0x24, 0xF0,
    0x48, 0x8B, 0x44, 0x24, 0x08,
    0x48, 0x89, 0x44, 0x24, 0xF8,
    0x48, 0x8B, 0x44, 0x24, 0x10,
    0x48, 0x89, 0x04, 0x24,
    0x48, 0x8B, 0x44, 0x24, 0x18,
    0x48, 0x89, 0x44, 0x24, 0x08,
    0x48, 0x8B, 0x44, 0x24, 0x20,
    0x48, 0x89, 0x44, 0x24, 0x10,
    0x48, 0x8B, 0x44, 0x24, 0xF0,
    0x48, 0x89, 0x44, 0x24, 0x18,
    0x48, 0xB8, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
    0x48, 0x89, 0x44, 0x24, 0x20,
    0x48, 0x83, 0xEC, 0x08,
    0x48, 0xB8, 0x20, 0x11, 0x87, 0x3F, 0x01, 0x00, 0x00, 0x00,
    0xFF, 0xD0, 
    0x4C, 0x8B, 0x44, 0x24, 0x20,
    0x4C, 0x89, 0x44, 0x24, 0xF8,
    0x4C, 0x8B, 0x44, 0x24, 0x18,
    0x4C, 0x89, 0x44, 0x24, 0x28,
    0x4C, 0x8B, 0x44, 0x24, 0x10,
    0x4C, 0x89, 0x44, 0x24, 0x20,
    0x4C, 0x8B, 0x44, 0x24, 0x08,
    0x4C, 0x89, 0x44, 0x24, 0x18,
    0x4C, 0x8B, 0x04, 0x24,
    0x4C, 0x89, 0x44, 0x24, 0x10,
    0x4C, 0x8B, 0x44, 0x24, 0xF8,
    0x48, 0x83, 0xC4, 0x10,
    0x41, 0x50, 
    0xC3,
  };     

  // INFO RAII Helper to VirtualProtect
  class WriteableProtect
  {
  public:
    WriteableProtect(uintptr_t addr, int size)
      : _target((void*)addr)
      , _size(size)
    {
      VirtualProtect(_target, _size, PAGE_EXECUTE_READWRITE, &_oldProtect);
    }

    ~WriteableProtect()
    {
      DWORD oldprot1;
      VirtualProtect(_target, _size, _oldProtect, &oldprot1);
    }

  private:
    void* _target;
    int _size;
    DWORD _oldProtect;
  };

  class HookEnginePrivate {
  public:
    HookEnginePrivate()
    {
    }

    ~HookEnginePrivate()
    {
      auto it = this->_hookes.begin();
      auto end = this->_hookes.end();

      for (; it != end; ++it)
        delete it->second;
    }

    bool getHook(uintptr_t realAddr, HookInfo **hook)
    {
      if (hook == NULL)
        return false;

      auto iter = this->_hookes.find(realAddr);
      if (iter == this->_hookes.end())
        return false;

      *hook = (*iter).second;
      return true;
    }

    int getCodeSize(uintptr_t code, int minimumSize, int avaiableSize = 30)
    {
      int curLen = 0;
      ud_t ud_obj;
      ud_init(&ud_obj);
      ud_set_syntax(&ud_obj, NULL);
      ud_set_mode(&ud_obj, 64);
      ud_set_input_buffer(&ud_obj, (const uint8_t *)code, avaiableSize);
      while (ud_disassemble(&ud_obj)) {
        if (ud_insn_mnemonic(&ud_obj) == UD_Iinvalid) {
          return 0;
        }

        curLen += ud_insn_len(&ud_obj);
        if (curLen >= minimumSize)
          break;
      }

      return curLen >= minimumSize ? curLen : 0;
    }

    bool installHook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo **hookInfo)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, &cave))
        return false;

      uintptr_t hookProlog = cave;
      uintptr_t afterHook = cave + 32;
      char* hookPrologPtr = reinterpret_cast<char*>(cave);
      char* afterHookPtr = reinterpret_cast<char*>(afterHook);
      char* beforePtr = reinterpret_cast<char*>(realAddr);

      int moveableSize = this->getCodeSize(realAddr, 5);
      if (moveableSize <= 0)
        return false;

      bool realPrologFixed = false;

      if (moveableSize == 5) {
        if (*(uint8_t*)(beforePtr) == 0xE9) {
          realPrologFixed = true;

          int32_t offset = *(int32_t*)(realAddr + 1);
          uintptr_t actuallFunctionStart = realAddr + offset + 5;

          *(uint16_t*)(afterHookPtr) = 0xB848; // mov rax, ...
          *(uintptr_t*)(afterHookPtr + 2) = actuallFunctionStart;
          *(uint16_t*)(afterHookPtr + 2 + sizeof(uintptr_t)) = 0xE0FF; // jmp rax
        }
      }
      if (moveableSize == 7) {
        if (*(uint8_t*)(beforePtr) == 0x48
          && *(uint8_t*)(beforePtr + 1) == 0xff
          && *(uint8_t*)(beforePtr + 2) == 0x25) {
          realPrologFixed = true;

          int32_t offset = *(int32_t*)(realAddr + 3);
          uintptr_t functionStartPointer = realAddr + offset + 7;
          uintptr_t actuallFunctionStart = *(uintptr_t*)(functionStartPointer);

          *(uint16_t*)(afterHookPtr) = 0xB848; // mov rax, ...
          *(uintptr_t*)(afterHookPtr + 2) = actuallFunctionStart;
          *(uint16_t*)(afterHookPtr + 2 + sizeof(uintptr_t)) = 0xE0FF; // jmp rax
        }
      }

      if (!realPrologFixed) {
        // UNDONE Тут бы надо проверить опкоды на возможность перемещения их
        memcpy(afterHookPtr, (LPVOID)realAddr, moveableSize);

        afterHookPtr[moveableSize] = 0xE9; //jmp
        //int jmpAddr = (realAddr + moveableSize) - ((uintptr_t)afterHookPtr + moveableSize) - 5;
        // После сокращения получается:
        int32_t jmpAddr = (int32_t)(realAddr - (uintptr_t)afterHookPtr - 5);
        *(int32_t*)(afterHookPtr + moveableSize + 1) = jmpAddr;
      }

      // INFO prolog:
      *(uint16_t*)(hookPrologPtr) = 0xB848; // mov rax, ...
      *(uintptr_t*)(hookPrologPtr + 2) = hookAddr;
      *(uint16_t*)(hookPrologPtr + 2 + sizeof(uintptr_t)) = 0xE0FF; // jmp rax

      WriteableProtect protect(realAddr, moveableSize + 5);
      int32_t prologJmp = (int32_t)(hookProlog - realAddr - 5);
      *beforePtr = 0xE9;
      *(int32_t*)(beforePtr + 1) = prologJmp;

      HookInfo* result = new HookInfo();
      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(afterHook);

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
    }

    // *------------------------------
    //  000000013F8711A6 | 48 8B 04 24    | mov rax, qword ptr ss : [rsp] |
    //  000000013F8711AA | 48 89 44 24 F0 | mov qword ptr ss : [rsp - 10], rax |
    //  000000013F8711AF | 48 8B 44 24 08 | mov rax, qword ptr ss : [rsp + 8] |
    //  000000013F8711B4 | 48 89 44 24 F8 | mov qword ptr ss : [rsp - 8], rax |
    //  000000013F8711B9 | 48 8B 44 24 10 | mov rax, qword ptr ss : [rsp + 10] |
    //  000000013F8711BE | 48 89 04 24    | mov qword ptr ss : [rsp], rax |
    //  000000013F8711C2 | 48 8B 44 24 18 | mov rax, qword ptr ss : [rsp + 18] |
    //  000000013F8711C7 | 48 89 44 24 08 | mov qword ptr ss : [rsp + 8], rax |
    //  000000013F8711CC | 48 8B 44 24 20 | mov rax, qword ptr ss : [rsp + 20] |
    //  000000013F8711D1 | 48 89 44 24 10 | mov qword ptr ss : [rsp + 10], rax |
    //  000000013F8711D6 | 48 8B 44 24 F0 | mov rax, qword ptr ss : [rsp - 10] |
    //  000000013F8711DB | 48 89 44 24 18 | mov qword ptr ss : [rsp + 18], rax |
    //  000000013F8711E0 | 48 B8 07 06 05 04 03 02 01 00 | mov rax, 1020304050607 |
    //  000000013F8711EA | 48 89 44 24 20 | mov qword ptr ss : [rsp + 20], rax |
    //  000000013F8711EF | 48 83 EC 08    | sub rsp, 8 |
    //  000000013F8711F3 | 48 B8 20 11 87 3F 01 00 00 00 | mov rax, <test_win64hook.target1Hook> |
    //  000000013F8711FD | FF D0          | call rax |
    //  000000013F8711FF | 4C 8B 44 24 20 | mov r8, qword ptr ss : [rsp + 20] |
    //  000000013F871204 | 4C 89 44 24 F8 | mov qword ptr ss : [rsp - 8], r8 |
    //  000000013F871209 | 4C 8B 44 24 18 | mov r8, qword ptr ss : [rsp + 18] |
    //  000000013F87120E | 4C 89 44 24 28 | mov qword ptr ss : [rsp + 28], r8 |
    //  000000013F871213 | 4C 8B 44 24 10 | mov r8, qword ptr ss : [rsp + 10] |
    //  000000013F871218 | 4C 89 44 24 20 | mov qword ptr ss : [rsp + 20], r8 |
    //  000000013F87121D | 4C 8B 44 24 08 | mov r8, qword ptr ss : [rsp + 8] |
    //  000000013F871222 | 4C 89 44 24 18 | mov qword ptr ss : [rsp + 18], r8 |
    //  000000013F871227 | 4C 8B 04 24    | mov r8, qword ptr ss : [rsp] |
    //  000000013F87122B | 4C 89 44 24 10 | mov qword ptr ss : [rsp + 10], r8 |
    //  000000013F871230 | 4C 8B 44 24 F8 | mov r8, qword ptr ss : [rsp - 8] |
    //  000000013F871235 | 48 83 C4 10    | add rsp, 10 |
    //  000000013F871239 | 41 50          | push r8 |
    //  000000013F87123B | C3             | ret |
    // -------------------------------
    bool installHookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= nullptr*/)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, &cave))
        return false;

      uintptr_t hookProlog = cave;
      uintptr_t afterHook = cave + 0xA0;
      char* hookPrologPtr = reinterpret_cast<char*>(cave);
      char* afterHookPtr = reinterpret_cast<char*>(afterHook);
      char* beforePtr = reinterpret_cast<char*>(realAddr);

      int moveableSize = this->getCodeSize(realAddr, 5);
      if (moveableSize <= 0)
        return false;

      bool realPrologFixed = false;

      // UNDONE исправление стандартных джамов
      if (moveableSize == 5) {
        if (*(uint8_t*)(beforePtr) == 0xE9) {
          realPrologFixed = true;

          int32_t offset = *(int32_t*)(realAddr + 1);
          uintptr_t actuallFunctionStart = realAddr + offset + 5;

          *(uint16_t*)(afterHookPtr) = 0xB848; // mov rax, ...
          *(uintptr_t*)(afterHookPtr + 2) = actuallFunctionStart;
          *(uint16_t*)(afterHookPtr + 2 + sizeof(uintptr_t)) = 0xE0FF; // jmp rax
        }
      }
      if (moveableSize == 7) {
        if (*(uint8_t*)(beforePtr) == 0x48
          && *(uint8_t*)(beforePtr + 1) == 0xff
          && *(uint8_t*)(beforePtr + 2) == 0x25) {
          realPrologFixed = true;

          int32_t offset = *(int32_t*)(realAddr + 3);
          uintptr_t functionStartPointer = realAddr + offset + 7;
          uintptr_t actuallFunctionStart = *(uintptr_t*)(functionStartPointer);

          *(uint16_t*)(afterHookPtr) = 0xB848; // mov rax, ...
          *(uintptr_t*)(afterHookPtr + 2) = actuallFunctionStart;
          *(uint16_t*)(afterHookPtr + 2 + sizeof(uintptr_t)) = 0xE0FF; // jmp rax
        }
      }

      if (!realPrologFixed) {
        // UNDONE Тут бы надо проверить опкоды на возможность перемещения их
        memcpy(afterHookPtr, (LPVOID)realAddr, moveableSize);

        afterHookPtr[moveableSize] = 0xE9; //jmp
        //int jmpAddr = (realAddr + moveableSize) - ((uintptr_t)afterHookPtr + moveableSize) - 5;
        // После сокращения получается:
        int32_t jmpAddr = (int32_t)(realAddr - (uintptr_t)afterHookPtr - 5);
        *(int32_t*)(afterHookPtr + moveableSize + 1) = jmpAddr;
      }

      HookInfo* result = new HookInfo();
      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(afterHook);


      // INFO prolog:
      memcpy(hookPrologPtr, templateProlog, sizeof(templateProlog));
      *(uintptr_t*)(hookPrologPtr + 0x3C) = (uintptr_t)result;
      *(uintptr_t*)(hookPrologPtr + 0x4F) = hookAddr;

      WriteableProtect protect(realAddr, moveableSize + 5);
      int32_t prologJmp = (int32_t)(hookProlog - realAddr - 5);
      *beforePtr = 0xE9;
      *(int32_t*)(beforePtr + 1) = prologJmp;

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
    }

    CodeCaveFinder _codeCaveFinder;
    std::map<uintptr_t, HookInfo *> _hookes;
    System::CriticalSection _criticalSection;
  };

  HookEngine::HookEngine()
    : _d(new HookEnginePrivate)
  {
  }

  HookEngine::~HookEngine()
  {
    delete _d;
  }

  bool HookEngine::installHook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= nullptr*/)
  {
    return this->_d->installHook(realAddr, hookAddr, hookInfo);
  }

  bool HookEngine::installHookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= nullptr*/)
  {
    return this->_d->installHookForTemplate(realAddr, hookAddr, hookInfo);
  }

}

#endif