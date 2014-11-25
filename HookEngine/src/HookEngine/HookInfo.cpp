#include <HookEngine/HookInfo.h>

namespace HookEngine {

  HookInfo::HookInfo()
    : _beforePtr(0)
    , _afterPtr(0)
    , _hookPtr(0)
    , _prologPtr(0)
    , _receiver(0)
    , _baseOffset(0)
  {
  }

  HookInfo::~HookInfo()
  {
  }

  uint32_t HookInfo::beforePtr() const
  {
    return this->_beforePtr;
  }

  void HookInfo::setBeforePtr(uint32_t value)
  {
    this->_beforePtr = value;
  }

  uint32_t HookInfo::afterPtr() const
  {
    return this->_afterPtr;
  }

  void HookInfo::setAfterPtr(uint32_t value)
  {
    this->_afterPtr = value;
  }

  uint32_t HookInfo::hookPtr() const
  {
    return this->_hookPtr;
  }

  void HookInfo::setHookPtr(uint32_t value)
  {
    this->_hookPtr = value;
  }

  void* HookInfo::receiver() const
  {
    return this->_receiver;
  }

  void HookInfo::setReceiver(void *value)
  {
    this->_receiver = value;
  }

  uint32_t HookInfo::prologPtr() const
  {
    return this->_prologPtr;
  }

  void HookInfo::setPrologPtr(uint32_t value)
  {
    this->_prologPtr = value;
  }

}