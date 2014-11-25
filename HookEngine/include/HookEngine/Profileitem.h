#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <stdint.h>

#include <HookEngine/System/Singleton.h>

namespace HookEngine {

  class ProfileItem;

  class Profiler : public System::Singleton<Profiler>
  {
  public:
    Profiler();
    ~Profiler();

    void registerItem(ProfileItem *item);
    std::string result();

  private:
    std::vector<ProfileItem *> _items;
  };

  class ProfileItem {
  public:
    ProfileItem();

    ~ProfileItem() {}

    void beginCall();
    void endCall();

    void originalBeginCall();
    void originalEndCall();

    void setAddress(uint32_t value);

    uint32_t addr() const;
    uint64_t callCount() const;
    double totalTime() const;
    double maxCallTime() const;

  private:
    uint32_t _addr;
    uint64_t _callCount;
    double _totalTime;
    double _maxCallTime;
    double _totalOriginalTime;
    LARGE_INTEGER _frequency;
    LARGE_INTEGER _startTime;
    LARGE_INTEGER _startOriginalCall;
  };

  class ProfilerWatcher {
  public:
    ProfilerWatcher(ProfileItem *item);
    ~ProfilerWatcher();

  private:
    ProfileItem *_item;
  };

  class ProfilerOriginalWatcher {
  public:
    ProfilerOriginalWatcher(ProfileItem *item);
    ~ProfilerOriginalWatcher();

  private:
    ProfileItem *_item;
  };

}