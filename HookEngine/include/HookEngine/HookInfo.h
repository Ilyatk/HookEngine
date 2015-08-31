#pragma once

#include <stdint.h>

namespace HookEngine {

  class HookInfo
  {
  public:
    HookInfo();
    ~HookInfo();

    // Original address of hooked function
    uintptr_t beforePtr() const;
    void setBeforePtr(uintptr_t value);

    // New address of hooked function
    uintptr_t afterPtr() const;
    void setAfterPtr(uintptr_t value);

    // Address of hook
    uintptr_t hookPtr() const;
    void setHookPtr(uintptr_t value);

    uintptr_t prologPtr() const;
    void setPrologPtr(uintptr_t value);

    void *receiver() const;
    void setReceiver(void *value);

  private:
    uintptr_t _beforePtr;
    uintptr_t _afterPtr;
    uintptr_t _hookPtr;
    uintptr_t _prologPtr;
    void *_receiver;
  };

}
