// INFO private X64 implementation
#pragma once

#ifdef _WIN64

#include <HookEngine/HookInfo.h>
#include <HookEngine/System/Singleton.h>
#include <HookEngine/System/CriticalSection.h>

#include <stdint.h>

namespace HookEngine {

  class HookEnginePrivate;
  class HookEngine
  {
  public:
    HookEngine();
    ~HookEngine();

    bool installHook(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo = nullptr);

    // For internal use only
    bool installHookForTemplate(uintptr_t realAddr, uintptr_t hookAddr, HookInfo** hookInfo = nullptr);

  private:
    HookEnginePrivate* _d;
  };
}

#endif