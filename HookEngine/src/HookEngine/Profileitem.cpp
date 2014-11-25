
#include <HookEngine/Profileitem.h>
#include <sstream>
#include <iomanip>

namespace HookEngine {

  Profiler::Profiler() {

  }

  Profiler::~Profiler() {

  }

  void Profiler::registerItem(ProfileItem *item)
  {
    this->_items.push_back(item);
  }

  std::string Profiler::result()
  {
    std::stringstream ss;
    for (int i = 0; i < this->_items.size(); ++i) {
      ProfileItem *item = this->_items[i];
      ss << "Addr: " << std::hex << std::showbase << item->addr() 
        << std::dec << "\tCall Count: " << item->callCount()
        << "\tTotalTime: " << std::fixed << std::setw( 20 ) << std::setprecision( 6 ) << item->totalTime()
        << "\tMaxTime: " << std::fixed << std::setw( 20 ) << std::setprecision( 6 ) << item->maxCallTime() 
        << std::endl;
    }

    return ss.str();
  }

  ProfileItem::ProfileItem() : _callCount(0)
    , _totalTime(0)
    , _addr(0)
    , _maxCallTime(0)
  {
    QueryPerformanceFrequency(&_frequency);
  }

  void ProfileItem::beginCall()
  {
    this->_callCount++;
    this->_totalOriginalTime = 0;
    QueryPerformanceCounter(&this->_startTime);
  }

  void ProfileItem::endCall()
  {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);

    double callTime = 1000 * ((double)(li.QuadPart-this->_startTime.QuadPart)/this->_frequency.QuadPart);
    callTime -= this->_totalOriginalTime;
    this->_totalTime += callTime;
    this->_maxCallTime = this->_maxCallTime > callTime ? this->_maxCallTime : callTime;
  }

  void ProfileItem::originalBeginCall()
  {
    QueryPerformanceCounter(&this->_startOriginalCall);
  }

  void ProfileItem::originalEndCall()
  {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    double callTime = 1000 * ((double)(li.QuadPart-this->_startOriginalCall.QuadPart)/this->_frequency.QuadPart);
    this->_totalOriginalTime += callTime;
  }

  void ProfileItem::setAddress(uint32_t value)
  {
    this->_addr = value;
  }

  uint32_t ProfileItem::addr() const
  {
    return this->_addr;
  }

  uint64_t ProfileItem::callCount() const
  {
    return this->_callCount;
  }

  double ProfileItem::totalTime() const
  {
    return this->_totalTime;
  }

  double ProfileItem::maxCallTime() const
  {
    return this->_maxCallTime;
  }



  ProfilerWatcher::ProfilerWatcher(ProfileItem *item) : _item(item)
  {
    this->_item->beginCall();
  }

  ProfilerWatcher::~ProfilerWatcher()
  {
    this->_item->endCall();
  }

  ProfilerOriginalWatcher::ProfilerOriginalWatcher(ProfileItem *item) : _item(item)
  {
    this->_item->originalBeginCall();
  }

  ProfilerOriginalWatcher::~ProfilerOriginalWatcher()
  {
    this->_item->originalEndCall();
  }

}