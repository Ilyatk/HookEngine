#pragma once

#include <HookEngine/System/CriticalSection.h>
#include <HookEngine/System/Lock.h>

namespace HookEngine {
  namespace System {

    template <class T>
    class Singleton
    {
      static T* _instance;

    protected:
      Singleton() {
      }

      virtual ~Singleton() {
        _instance = NULL;
      }

    public:
      static T* instance();
    };

    template <class T>
    T* Singleton<T>::_instance = NULL;

    template <class T>
    T* Singleton<T>::instance()
    {
      if (_instance)
        return _instance;

      static System::CriticalSection man;
      System::Lock lock(man);

      if (_instance)
        return _instance;

      _instance = new T;
      return _instance;
    }

  }
}
