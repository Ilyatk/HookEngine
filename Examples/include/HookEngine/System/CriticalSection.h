#pragma once

#include <Windows.h>

namespace HookEngine {
  namespace System {

    class CriticalSection
    {
    public:
      friend class Lock;

      CriticalSection();
      ~CriticalSection();

      CRITICAL_SECTION& get();

    private:
      CRITICAL_SECTION _cs;
    };

  }
}

