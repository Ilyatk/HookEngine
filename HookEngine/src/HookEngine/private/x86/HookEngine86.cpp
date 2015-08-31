#ifndef _WIN64

#include <HookEngine/private/x86/HookEngine86.h>
#include <HookEngine/private/x86/CodeCaveFinder86.h>

#include <udis86/udis86.h>
//  Remember CAVE_SIZE 64
#define ORIGINAL_FUNCTION_PROLOG_LEN 42

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace HookEngine {
   
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

    int getCodeSize(uintptr_t code, int minimumSize, int avaiableSize = 30)
    {
      int curLen = 0;
      ud_t ud_obj;
      ud_init(&ud_obj);
      ud_set_syntax(&ud_obj, NULL);
      ud_set_mode(&ud_obj, 32);
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
      if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
        return false;

      unsigned char *cavePtr = (unsigned char*)(cave);

      //int opplen = 0;
      //unsigned char * currentpReal = (unsigned char *)realAddr;
      //while (opplen < 5) {
      //  int len = mlde32(currentpReal);
      //  currentpReal = currentpReal + len;
      //  opplen += len;

      //  if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
      //    return false;
      //}

      int opplen = this->getCodeSize(realAddr, 5);
      if (opplen <= 0 || opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;

      unsigned char * currentpReal = (unsigned char *)(realAddr + opplen);

      // Prepare original function:
      // copy corrupted prolog
      memcpy(cavePtr, (LPVOID)realAddr, opplen);

      // E9 Fix
      if (cavePtr[0] == 0xE9)
        *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

      // set jump to original function after prolog
      cavePtr[opplen] = 0xE9; //jmp
      int jmpAddr = (uintptr_t)currentpReal - (cave + opplen) - 5;
      *(int *)(cave + opplen + 1) = jmpAddr;

      DWORD oldprot;
      DWORD oldprot1;

      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
      int hookJmpAddr = (int)hookAddr - (int)realAddr - 5;
      *(unsigned char *)realAddr = 0xE9;
      *(int *)((unsigned char *)realAddr + 1) = hookJmpAddr;
      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

      HookInfo* result = new HookInfo();
      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(cave);

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
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

    bool installStdHook(uintptr_t realAddr, uintptr_t intance, uintptr_t hookAddr, HookInfo** hookInfo)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
        return false;

      unsigned char *cavePtr = (unsigned char*)(cave);
      //int opplen = 0;
      //unsigned char* currentpReal = (unsigned char *)realAddr;
      //while (opplen < 5) {
      //  int len = mlde32(currentpReal);
      //  currentpReal = currentpReal + len;
      //  opplen += len;

      //  if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
      //    return false;
      //}

      int opplen = this->getCodeSize(realAddr, 5);
      if (opplen <= 0 || opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;

      unsigned char * currentpReal = (unsigned char *)(realAddr + opplen);

      // Prepare original function:
      // copy corrupted prolog
      memcpy((LPVOID)cave, (LPVOID)realAddr, opplen);

      if (cavePtr[0] == 0xE9)
        *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

      // set jump to original function after prolog
      ((unsigned char *)cave)[opplen] = 0xE9; //jmp
      int jmpAddr = (uintptr_t)currentpReal - (cave + opplen) - 5;
      *(int *)(cave + opplen + 1) = jmpAddr;

      // Prepare this call for hook
      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN] = 0xB9; // mov ecx,
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = intance;

      int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
      *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE9;
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

      DWORD oldprot;
      DWORD oldprot1;

      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
      int caveHookJmpAddr = (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN) - (int)realAddr - 5;
      *(unsigned char *)realAddr = 0xE9;
      *(int *)((unsigned char *)realAddr + 1) = caveHookJmpAddr;
      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

      HookInfo* result = new HookInfo();
      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(cave);

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
    }

    bool fasthook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= NULL*/)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
        return false;

      unsigned char *cavePtr = (unsigned char*)(cave);

      //int opplen = 0;
      //unsigned char * currentpReal = (unsigned char *)realAddr;
      //while (opplen < 5) {
      //  int len = mlde32(currentpReal);
      //  currentpReal = currentpReal + len;
      //  opplen += len;

      //  if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
      //    return false;
      //}

      int opplen = this->getCodeSize(realAddr, 5);
      if (opplen <= 0 || opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;

      unsigned char * currentpReal = (unsigned char *)(realAddr + opplen);

      // Prepare original function:
      // copy corrupted prolog
      memcpy(cavePtr, (LPVOID)realAddr, opplen);

      if (cavePtr[0] == 0xE9)
        *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

      // set jump to original function after prolog
      cavePtr[opplen] = 0xE9; //jmp
      int jmpAddr = (uintptr_t)currentpReal - (cave + opplen) - 5;
      *(int *)(cave + opplen + 1) = jmpAddr;

      // Prepare this call for hook
      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN] = 0xBA; // mov edx,
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = cave;

      int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
      *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE9;
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

      DWORD oldprot;
      DWORD oldprot1;

      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
      int caveHookJmpAddr = (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN) - (int)realAddr - 5;
      *(unsigned char *)realAddr = 0xE9;
      *(int *)((unsigned char *)realAddr + 1) = caveHookJmpAddr;
      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

      HookInfo* result = new HookInfo();
      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(cave);

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
    }

    // *------------------------------
    // 1. Simple hook
    //     
    // Real
    //  jmp <hook1>
    //  ... garbage
    //  <point1>End of original function
    // 
    // Cave:
    //  - <cave_start>Some operand from original function
    //  - jmp to <point1>
    //
    // Hook:
    //  <hook1>
    //  
    // 2. Fast __fastcall function with edx = original function
    // Real:
    //  jmp <ORIGINAL_FUNCTION_PROLOG_LEN>
    //  ... garbage
    //  <point1>End of original function
    //  
    // Cave:
    //  - <cave_addr>Some operand from original function
    //  - jmp to <point1>
    //  ... garbege
    //  - <ORIGINAL_FUNCTION_PROLOG_LEN> mov edx, <cave_addr>
    //  - jmp <hook1>
    //   
    // Hook:
    //  <hook1>
    //  
    // 3. Fast __fastcall function with edx = original function and third arg = recivers
    // Real:
    //  jmp <ORIGINAL_FUNCTION_PROLOG_LEN>
    //  ... garbage
    //  <point1>End of original function
    //  
    // Cave:
    //  - <cave_addr>Some operand from original function
    //  - jmp to <point1>
    //  ... garbege
    //  - <ORIGINAL_FUNCTION_PROLOG_LEN> mov edx, <cave_addr>
    //  - push <reciever> 68 78 56 34 12
    //  - jmp <hook1>
    //   
    // Hook:
    //  <hook1>
    // -------------------------------

    bool fasthookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
        return false;

      unsigned char *cavePtr = (unsigned char*)(cave);

      //int opplen = 0;
      //unsigned char * currentpReal = (unsigned char *)realAddr;
      //while (opplen < 5) {
      //  int len = mlde32(currentpReal);
      //  currentpReal = currentpReal + len;
      //  opplen += len;

      //  if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
      //    return false;
      //}

      int opplen = this->getCodeSize(realAddr, 5);
      if (opplen <= 0 || opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;

      unsigned char * currentpReal = (unsigned char *)(realAddr + opplen);

      // Prepare original function:
      // copy corrupted prolog
      memcpy(cavePtr, (LPVOID)realAddr, opplen);

      if (cavePtr[0] == 0xE9)
        *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

      // set jump to original function after prolog
      cavePtr[opplen] = 0xE9; //jmp
      int jmpAddr = (uintptr_t)currentpReal - (cave + opplen) - 5;
      *(int *)(cave + opplen + 1) = jmpAddr;

      HookInfo* result = new HookInfo();

      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(cave);
      result->setPrologPtr(cave + ORIGINAL_FUNCTION_PROLOG_LEN);

      // Prepare this call for hook
      // CPU Disasm
      // 004905A2      58            POP EAX
      // 004905A3      68 44332211   PUSH 11223344 (receiver)
      // 004905A8      50            PUSH EAX

      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN] = 0x58; // pop eax

      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 1] = 0x68; // push receiver
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 2) = (uintptr_t)result;

      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 6] = 0x50; // push eax,

      int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7) - 5;
      *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7) = 0xE9;
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7 + 1) = hookJmpAddr;

      DWORD oldprot;
      DWORD oldprot1;

      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
      int caveHookJmpAddr = (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN) - (int)realAddr - 5;

      *(unsigned char *)realAddr = 0xE9;
      *(int *)((unsigned char *)realAddr + 1) = caveHookJmpAddr;
      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

      this->_hookes[realAddr] = result;

      if (hookInfo != NULL)
        *hookInfo = result;

      return true;
    }

    bool fasthookForTemplateCdecl(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo)
    {
      System::Lock lock(this->_criticalSection);

      HookInfo* oldHook;
      if (this->getHook(realAddr, &oldHook)) {
        if (hookInfo != NULL)
          *hookInfo = oldHook;

        return false;
      }

      uintptr_t cave;
      if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
        return false;

      unsigned char *cavePtr = (unsigned char*)(cave);

      //int opplen = 0;
      //unsigned char * currentpReal = (unsigned char *)realAddr;
      //while (opplen < 5) {
      //  int len = mlde32(currentpReal);
      //  currentpReal = currentpReal + len;
      //  opplen += len;

      //  if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
      //    return false;
      //}

      int opplen = this->getCodeSize(realAddr, 5);
      if (opplen <= 0 || opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;

      unsigned char * currentpReal = (unsigned char *)(realAddr + opplen);

      // Prepare original function:
      // copy corrupted prolog
      memcpy(cavePtr, (LPVOID)realAddr, opplen);

      if (cavePtr[0] == 0xE9)
        *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

      // set jump to original function after prolog
      cavePtr[opplen] = 0xE9; //jmp
      int jmpAddr = (uintptr_t)currentpReal - (cave + opplen) - 5;
      *(int *)(cave + opplen + 1) = jmpAddr;

      HookInfo* result = new HookInfo();

      result->setHookPtr(hookAddr);
      result->setBeforePtr(realAddr);
      result->setAfterPtr(cave);
      result->setPrologPtr(cave + ORIGINAL_FUNCTION_PROLOG_LEN);

      // Prepare this call for hook
      // CPU Disasm
      //  77E801B8   68 78563412      PUSH 12345678
      //  77E801BD   E8 94322A88      CALL 00123456
      //  77E801C2   59               POP ECX
      //  77E801C3   C3               RETN

      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 0] = 0x68; // push receiver
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = (uintptr_t)result;

      int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
      *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE8;
      *(uintptr_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 10] = 0x59; // pop ecx
      ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 11] = 0xC3; // retn

      DWORD oldprot;
      DWORD oldprot1;

      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
      int caveHookJmpAddr = (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN) - (int)realAddr - 5;
      *(unsigned char *)realAddr = 0xE9;
      *(int *)((unsigned char *)realAddr + 1) = caveHookJmpAddr;
      VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

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

  void HookEngine::init()
  {
  }

  bool HookEngine::fasthook(LPCSTR dllName, LPCSTR functionName, uintptr_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    HMODULE dll = GetModuleHandleA(dllName);
    if (!dll)
      return false;

    uintptr_t original = (uintptr_t)GetProcAddress(dll, functionName);
    if (!original)
      return false;

    return this->_d->fasthook(original, hookAddr, hookInfo);
  }

  bool HookEngine::fasthook(HMODULE dll, LPCSTR functionName, uintptr_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    uintptr_t original = (uintptr_t)GetProcAddress(dll, functionName);
    if (!original)
      return false;

    return this->_d->fasthook(original, hookAddr, hookInfo);
  }


  bool HookEngine::fasthook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    return this->_d->fasthook(realAddr, hookAddr, hookInfo);
  }

  bool HookEngine::installHook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    return this->_d->installHook(realAddr, hookAddr, hookInfo);
  }

  bool HookEngine::installStdHook(uintptr_t realAddr, uintptr_t intance, uintptr_t hookAddr, HookInfo** hookInfo)
  {
    return this->_d->installStdHook(realAddr, intance, hookAddr, hookInfo);
  }

  bool HookEngine::fasthookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo)
  {
    return this->_d->fasthookForTemplate(realAddr, hookAddr, hookInfo);
  }

  bool HookEngine::fasthookForTemplateCdecl(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo)
  {
    return this->_d->fasthookForTemplateCdecl(realAddr, hookAddr, hookInfo);
  }

}

#endif