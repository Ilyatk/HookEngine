#include <HookEngine/System/CriticalSection.h>

namespace HookEngine {
  namespace System {
    CriticalSection::CriticalSection()
    {
      InitializeCriticalSection(&this->_cs);
    }

    CriticalSection::~CriticalSection()
    {
      DeleteCriticalSection(&this->_cs);
    }

    CRITICAL_SECTION& CriticalSection::get()
    {
      return this->_cs;
    }
  }
}
