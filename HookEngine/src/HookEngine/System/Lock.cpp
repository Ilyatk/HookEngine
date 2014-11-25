#include <HookEngine/System/Lock.h>
#include <HookEngine/System/CriticalSection.h>

namespace HookEngine {
  namespace System {

    Lock::Lock(CriticalSection& cs)
      : _cs(cs._cs)
    {
      EnterCriticalSection(&this->_cs);
    }

    Lock::~Lock()
    {
      LeaveCriticalSection(&this->_cs);
    }

  }
}