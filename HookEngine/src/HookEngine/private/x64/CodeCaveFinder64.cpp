#include <HookEngine/private/x64/CodeCaveFinder64.h>

#ifdef _WIN64

#include <climits>

namespace HookEngine {

  class CaveInfo 
  {
  public:
    CaveInfo() 
      : startAddress(0)
      , currentAddress(0)
      , totalSize(0)
      , unusedSize(0)
    {
    }

    CaveInfo(
      uintptr_t startAddress_
      , uintptr_t currentAddress_
      , uintptr_t totalSize_
      , uintptr_t unusedSize_)
      : startAddress(startAddress_)
      , currentAddress(currentAddress_)
      , totalSize(totalSize_)
      , unusedSize(unusedSize_)
    {
    }

    ~CaveInfo()
    {
    }

    uintptr_t startAddress;
    uintptr_t currentAddress;
    uintptr_t totalSize;
    uintptr_t unusedSize;
  };

  CodeCaveFinder::CodeCaveFinder()
  {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    this->_pageSize = info.dwAllocationGranularity;
    this->_minimumApplicationAddress = reinterpret_cast<uintptr_t>(info.lpMinimumApplicationAddress);
    this->_maximumApplicationAddress = reinterpret_cast<uintptr_t>(info.lpMaximumApplicationAddress);
  }

  CodeCaveFinder::~CodeCaveFinder()
  {
  }

  bool CodeCaveFinder::getCodeCave(uintptr_t realAddr, uintptr_t *cave)
  {
    if (cave == NULL)
      return false;

    CaveInfo *caveInfo = NULL;
    if (!this->getFromCache(realAddr, &caveInfo)) {
      if (!this->allocateCave(realAddr, &caveInfo)) {
        return false;
      }
      else {
        this->_caves.push_front(caveInfo);
      }
    }

    *cave = this->consume(caveInfo, CAVE_SIZE);
    return true;
  }


  bool CodeCaveFinder::getCodeCave(uintptr_t *cave)
  {
    if (cave == NULL)
      return false;

    CaveInfo *caveInfo = NULL;

    auto it = this->_unattachedCaves.begin();
    auto end = this->_unattachedCaves.end();
    for (; it != end; ++it) {
      CaveInfo *cave = *it;
      if (cave->unusedSize < CAVE_SIZE)
        continue;

      caveInfo = cave;
      break;
    }

    if (!caveInfo) {
      LPVOID cave = VirtualAlloc(nullptr, this->_pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
      if (!cave)
        return false;

      caveInfo = new CaveInfo((uintptr_t)cave, (uintptr_t)cave, this->_pageSize, this->_pageSize);
      this->_unattachedCaves.push_front(caveInfo);
    }

    *cave = this->consume(caveInfo, CAVE_SIZE);
    return true;
  }

  bool CodeCaveFinder::allocateCave(uintptr_t realAddr, CaveInfo **caveInfo)
  {
    uintptr_t epsilon = 2000000000;
    uintptr_t minAddr = (this->_minimumApplicationAddress + epsilon) > realAddr
      ? this->_minimumApplicationAddress
      : realAddr - epsilon;

    uintptr_t maxAddr = (this->_maximumApplicationAddress - epsilon) < realAddr
      ? this->_maximumApplicationAddress
      : realAddr + epsilon;

    MEMORY_BASIC_INFORMATION memoryInfo;
    uintptr_t startPoint = realAddr - realAddr % this->_pageSize;
    uintptr_t currentAddr = startPoint;

    minAddr = minAddr - (minAddr % this->_pageSize) + this->_pageSize;
    maxAddr = maxAddr - (maxAddr % this->_pageSize);

    while (true) {
      if (currentAddr <= minAddr)
        break;

      VirtualQuery((LPCVOID)currentAddr, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
      if (memoryInfo.State == MEM_FREE) {
        LPVOID cave = VirtualAlloc((LPVOID)currentAddr, this->_pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!cave)
          continue;

        *caveInfo = new CaveInfo((uintptr_t)cave, (uintptr_t)cave, this->_pageSize, this->_pageSize);
        return true;
      }

      currentAddr -= this->_pageSize;
    }

    currentAddr = startPoint;

    while (true) {
      if (currentAddr >= maxAddr)
        break;

      VirtualQuery((LPCVOID)currentAddr, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
      if (memoryInfo.State == MEM_FREE) {
        LPVOID cave = VirtualAlloc((LPVOID)currentAddr, this->_pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!cave)
          continue;

        *caveInfo = new CaveInfo((uintptr_t)cave, (uintptr_t)cave, this->_pageSize, this->_pageSize);
        return true;
      }

      currentAddr += this->_pageSize;
    }

    return false;
  }

  bool CodeCaveFinder::getFromCache(uintptr_t realAddr, CaveInfo **caveInfo)
  {
    uintptr_t epsilon = 2000000000;

    uintptr_t current;
    uintptr_t diff;
    auto it = this->_caves.begin();
    auto end = this->_caves.end();
    for (; it != end; ++it) {
      CaveInfo *cave = *it;
      if (cave->unusedSize < CAVE_SIZE)
        continue;

      current = cave->currentAddress;
      diff = current > realAddr
        ? current - realAddr
        : realAddr - current;

      if (diff > epsilon)
        continue;

      *caveInfo = cave;
      return true;
    }

    return false;
  }

  uintptr_t CodeCaveFinder::consume(CaveInfo *caveInfo, size_t caveSize)
  {
    uintptr_t result = caveInfo->currentAddress;

    caveInfo->currentAddress += caveSize;
    caveInfo->unusedSize -= caveSize;

    return result;
  }

}

#endif