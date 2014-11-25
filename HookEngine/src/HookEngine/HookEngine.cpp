#include <HookEngine/HookEngine.h>

#include <HookEngine/System/Lock.h>

//  Remember CAVE_SIZE 64
#define ORIGINAL_FUNCTION_PROLOG_LEN 42

namespace HookEngine {
  HookEngine::HookEngine()
  {
  }

  HookEngine::~HookEngine()
  {
    auto it = this->_hookes.begin();
    auto end = this->_hookes.end();

    for (; it != end; ++it)
      delete it->second;
  }

  void HookEngine::init()
  {
    this->_loadLibraryHooks.init();
  }

  bool HookEngine::installHook(uint32_t realAddr, uint32_t hookAddr, HookInfo **hookInfo)
  {
    System::Lock lock(this->_criticalSection);

    HookInfo* oldHook;
    if (this->getHook(realAddr, &oldHook)) {
      if (hookInfo != NULL)
        *hookInfo = oldHook;

      return false;
    }

    uint32_t cave;
    if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
      return false;

    unsigned char *cavePtr = (unsigned char*)(cave);
    int opplen = 0;
    unsigned char * currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy(cavePtr, (LPVOID)realAddr, opplen);

    // E9 Fix
    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    cavePtr[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
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

  bool HookEngine::getHook(uint32_t realAddr, HookInfo **hook)
  {
    if (hook == NULL)
      return false;

    auto iter = this->_hookes.find(realAddr);
    if (iter == this->_hookes.end())
      return false;

    *hook = (*iter).second;
    return true;
  }

  bool HookEngine::installStdHook(uint32_t realAddr, uint32_t intance, uint32_t hookAddr, HookInfo** hookInfo)
  {
    System::Lock lock(this->_criticalSection);

    HookInfo* oldHook;
    if (this->getHook(realAddr, &oldHook)) {
      if (hookInfo != NULL)
        *hookInfo = oldHook;

      return false;
    }

    uint32_t cave;
    if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
      return false;

    unsigned char *cavePtr = (unsigned char*)(cave);
    int opplen = 0;
    unsigned char* currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy((LPVOID)cave, (LPVOID)realAddr, opplen);

    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    ((unsigned char *)cave)[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
    *(int *)(cave + opplen + 1) = jmpAddr;

    // Prepare this call for hook
    ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN] = 0xB9; // mov ecx,
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = intance;

    int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
    *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE9;
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

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

  bool HookEngine::fasthook(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    System::Lock lock(this->_criticalSection);

    HookInfo* oldHook;
    if (this->getHook(realAddr, &oldHook)) {
      if (hookInfo != NULL)
        *hookInfo = oldHook;

      return false;
    }

    uint32_t cave;
    if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
      return false;

    unsigned char *cavePtr = (unsigned char*)(cave);

    int opplen = 0;
    unsigned char * currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy(cavePtr, (LPVOID)realAddr, opplen);

    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    cavePtr[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
    *(int *)(cave + opplen + 1) = jmpAddr;

    // Prepare this call for hook
    ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN] = 0xBA; // mov edx,
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = cave;

    int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
    *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE9;
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

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

  bool HookEngine::fasthookForTemplate(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo)
  {
    System::Lock lock(this->_criticalSection);

    HookInfo* oldHook;
    if (this->getHook(realAddr, &oldHook)) {
      if (hookInfo != NULL)
        *hookInfo = oldHook;

      return false;
    }

    uint32_t cave;
    if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
      return false;

    unsigned char *cavePtr = (unsigned char*)(cave);

    int opplen = 0;
    unsigned char * currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy(cavePtr, (LPVOID)realAddr, opplen);

    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    cavePtr[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
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
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 2) = (uint32_t)result;

    ((unsigned char *)cave)[ORIGINAL_FUNCTION_PROLOG_LEN + 6] = 0x50; // push eax,

    int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7) - 5;
    *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7) = 0xE9;
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 7 + 1) = hookJmpAddr;

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

  bool HookEngine::fasthookForTemplateCdecl(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo)
  {
    System::Lock lock(this->_criticalSection);

    HookInfo* oldHook;
    if (this->getHook(realAddr, &oldHook)) {
      if (hookInfo != NULL)
        *hookInfo = oldHook;

      return false;
    }

    uint32_t cave;
    if (!this->_codeCaveFinder.getCodeCave(realAddr, hookAddr, &cave))
      return false;

    unsigned char *cavePtr = (unsigned char*)(cave);

    int opplen = 0;
    unsigned char * currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return false;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy(cavePtr, (LPVOID)realAddr, opplen);

    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    cavePtr[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
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
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 1) = (uint32_t)result;

    int hookJmpAddr = (int)hookAddr - (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) - 5;
    *(unsigned char *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5) = 0xE8;
    *(uint32_t *)(cave + ORIGINAL_FUNCTION_PROLOG_LEN + 5 + 1) = hookJmpAddr;

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

  bool HookEngine::fasthook(LPCSTR dllName, LPCSTR functionName, uint32_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    HMODULE dll = GetModuleHandleA(dllName);
    if (!dll)
      return false;

    uint32_t original = (uint32_t)GetProcAddress(dll, functionName);
    if (!original)
      return false;

    return this->fasthook(original, hookAddr, hookInfo);
  }

  bool HookEngine::fasthook(HMODULE dll, LPCSTR functionName, uint32_t hookAddr, HookInfo** hookInfo /*= NULL*/)
  {
    uint32_t original = (uint32_t)GetProcAddress(dll, functionName);
    if (!original)
      return false;

    return this->fasthook(original, hookAddr, hookInfo);
  }

  std::wstring HookEngine::getModuleFullPath(HMODULE module) 
  {
    std::vector<wchar_t> path(MAX_PATH);
    DWORD nsize = ::GetModuleFileNameW(module, &path[0], static_cast<DWORD>(path.size()));
    while(nsize == path.size()) {
      path.resize(path.size() * 2);
      nsize = ::GetModuleFileNameW(module, &path[0], static_cast<DWORD>(path.size()));
    }

    if (nsize == 0)
      return std::wstring();

    return std::wstring(path.begin(), path.begin() + nsize);
  }

  void HookEngine::registerForRestore(HookInfo *hookInfo)
  {
    MEMORY_BASIC_INFORMATION mbi;
    HMODULE dll = 0;
    if (!VirtualQuery((LPCVOID)hookInfo->_beforePtr, &mbi, sizeof(mbi)))  {
      this->log("registerForRestore Failed to get memory info");
      return;
    }

    dll = (HMODULE)mbi.AllocationBase;

    std::wstring fullPath(this->getModuleFullPath(dll));
    hookInfo->_modulePath = fullPath;
    hookInfo->_baseOffset = (int64_t)hookInfo->beforePtr() - (int64_t)mbi.AllocationBase;

    this->_activeHook[fullPath].push_back(hookInfo);
  }

  void HookEngine::fixTrampoline(int64_t baseAddress, HookInfo *info)
  {
    uint32_t realAddr = (uint32_t)(baseAddress + info->_baseOffset);
    uint32_t cave = info->afterPtr();

    unsigned char *cavePtr = (unsigned char*)(cave);
    int opplen = 0;
    unsigned char * currentpReal = (unsigned char *)realAddr;
    while (opplen < 5) {
      int len = mlde32(currentpReal);
      currentpReal = currentpReal + len;
      opplen += len;

      if (opplen > ORIGINAL_FUNCTION_PROLOG_LEN)
        return;
    }

    // Prepare original function:
    // copy corrupted prolog
    memcpy(cavePtr, (LPVOID)realAddr, opplen);

    if (cavePtr[0] == 0xE9) 
      *(int*)(cavePtr + 1) = realAddr - cave + *(int*)(cavePtr + 1);

    // set jump to original function after prolog
    cavePtr[opplen] = 0xE9; //jmp
    int jmpAddr = (uint32_t)currentpReal - (cave + opplen) - 5;
    *(int *)(cave + opplen + 1) = jmpAddr;

    DWORD oldprot;
    DWORD oldprot1;

    VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
    int caveHookJmpAddr = (int)(cave + ORIGINAL_FUNCTION_PROLOG_LEN) - (int)realAddr - 5;
    *(unsigned char *)realAddr = 0xE9;
    *(int *)((unsigned char *)realAddr + 1) = caveHookJmpAddr;
    VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);
  }

  void HookEngine::moduleLoaded(HMODULE module)
  {
    if (this->_onModuleLoaded)
      this->_onModuleLoaded();

    MEMORY_BASIC_INFORMATION mbi;

    for (auto it = this->_activeHook.begin(); it != this->_activeHook.end(); ++it) {
      HMODULE module = GetModuleHandleW(it->first.c_str());
      if (!module)
        continue;

      HMODULE dll = 0;
      if (!VirtualQuery((LPCVOID)module, &mbi, sizeof(mbi)))
        continue;

      for (unsigned int i = 0; i < it->second.size(); ++i) {
        HookInfo *info = it->second[i];

        if (this->isHookBroken((uint32_t)((int64_t)mbi.AllocationBase + info->_baseOffset), info->prologPtr())) {
          this->fixTrampoline((int64_t)mbi.AllocationBase, info);
        }
      }
    }
  }

  void HookEngine::moduleUnloaded(HMODULE module)
  {
    if (this->_onModuleUnloaded)
      this->_onModuleUnloaded();
  }

  void HookEngine::onModuleLoaded(std::function<void()> callback)
  {
    this->_onModuleLoaded = callback;
  }

  void HookEngine::onModuleUnloaded(std::function<void()> callback)
  {
    this->_onModuleUnloaded = callback;
  }

  void HookEngine::log(const std::string& msg)
  {
    if (!this->_externalLogger)
      return;

    this->_externalLogger(msg);
  }

  void HookEngine::setLogger(std::function<void(const std::string&)> logger)
  {
    this->_externalLogger = logger;
  }

  bool HookEngine::isHookBroken(uint32_t realAddr, uint32_t targetAddr)
  {
    DWORD oldprot;
    DWORD oldprot1;
    bool result = false;

    VirtualProtect((LPVOID)realAddr, CAVE_SIZE, PAGE_EXECUTE_READWRITE, &oldprot);
    int64_t offset = (int64_t)(*(uint32_t*)(realAddr + 1));
    uint32_t currentTarget = (uint32_t)((int64_t)realAddr + offset + 5);
    result = currentTarget != targetAddr;
    VirtualProtect((LPVOID)realAddr, CAVE_SIZE, oldprot, &oldprot1);

    return result;
  }

}