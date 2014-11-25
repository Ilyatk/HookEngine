#pragma once

#include <stdint.h>
#include <string>
#include <Windows.h>

namespace HookEngine {

  class HookInfo
  {
  public:
    friend class HookEngine;

    HookInfo();
    ~HookInfo();

    // Original address of hooked function
    uint32_t beforePtr() const;
    void setBeforePtr(uint32_t value);

    // New address of hooked function
    uint32_t afterPtr() const;
    void setAfterPtr(uint32_t value);

    // Address of hook
    uint32_t hookPtr() const;
    void setHookPtr(uint32_t value);

    uint32_t prologPtr() const;
    void setPrologPtr(uint32_t value);

    void *receiver() const;
    void setReceiver(void *value);

  private:
    uint32_t _beforePtr;
    uint32_t _afterPtr;
    uint32_t _hookPtr;
    uint32_t _prologPtr;
    void *_receiver;

    int64_t _baseOffset;
    std::wstring _modulePath;
  };

}
