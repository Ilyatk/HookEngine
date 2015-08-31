// INFO private X86 implementation
#pragma once

#ifndef _WIN64

//#include <HookEngine/Hook.h>
//#include <HookEngine/CodeCaveFinder.h>

#include <HookEngine/HookInfo.h>
#include <HookEngine/System/Singleton.h>
#include <HookEngine/System/CriticalSection.h>

#include <stdint.h>

namespace HookEngine {

  /*!
  \fn template<typename T> uintptr_t ClassFuntionPointerToUint32(T p)
  \brief Cast class member function address to uintptr_t.
  \author Ilya.Tkachenko
  \date 12.04.2013
  \param p The.
  \return .
  */
  template<typename T> uintptr_t ClassFuntionPointerToUint32(T p) {
    // INFO This code probably should be removed or moved somewhere else
    uintptr_t tmp;
    __asm {
      mov eax, p;
      mov tmp, eax;
    }
    return tmp;
  }

  class HookEnginePrivate;
  class HookEngine
  {
  public:
    HookEngine();
    ~HookEngine();

    void init();

    bool fasthook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo = NULL);
    bool fasthook(LPCSTR dllName, LPCSTR functionName, uintptr_t hookAddr, HookInfo** hookInfo = NULL);
    bool fasthook(HMODULE dll, LPCSTR functionName, uintptr_t hookAddr, HookInfo** hookInfo = NULL);

    // просто обертка чтобы не каститить перед вызовом функцию в uintptr_t
    template <typename T> bool fasthook(HMODULE dll, LPCSTR functionName, T hookFunc, HookInfo** hookInfo = NULL)
    {
      return this->fasthook(dll, functionName, (uintptr_t)hookFunc, hookInfo);
    }

    // хук общего назначения - просто ставит трамплин на указанный адресс не меня ничего
    bool installHook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo = NULL);

    // Устанавливает хук на __stdcall функцию. При этом вызывается instance->hookAddr 
    bool installStdHook(uintptr_t realAddr, uintptr_t intance, uintptr_t hookAddr, HookInfo** hookInfo);

    // WARNING be carefully with hookInfo - you shouldn't delete it.
    bool fasthookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo);
    bool fasthookForTemplateCdecl(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo);

  private:
    HookEnginePrivate* _d;

    //HookByModuleMap _activeHook;

    //CodeCaveFinder _codeCaveFinder;
    //std::map<uintptr_t, HookInfo *> _hookes;

    //System::CriticalSection _criticalSection;
    //LoadLibraryHook _loadLibraryHooks;

    //std::function<void()> _onModuleLoaded;
    //std::function<void()> _onModuleUnloaded;

    //std::function<void(const std::string& msg)> _externalLogger;

    //std::map<HMODULE, std::string> _modulesNameMap;

    //void moduleLoaded(HMODULE module);
    //void moduleUnloaded(HMODULE module);

    //void fixTrampoline(int64_t baseAddress, HookInfo *info);
    //bool isHookBroken(uintptr_t realAddr, uintptr_t targetAddr);

    //std::wstring getModuleFullPath(HMODULE module);
    //bool getHook(uintptr_t realAddr, HookInfo** hook);
  };
}

#endif