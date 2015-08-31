#include <HookEngine/private/x86/CodeCaveFinder86.h>

#ifndef _WIN64

#include <climits>

namespace HookEngine {

  CodeCaveFinder::CodeCaveFinder()
  {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    this->_pageSize = info.dwAllocationGranularity;
    this->_minimumApplicationAddress = info.lpMinimumApplicationAddress;
    this->_maximumApplicationAddress = info.lpMaximumApplicationAddress;
  }

  CodeCaveFinder::~CodeCaveFinder()
  {
  }

  bool CodeCaveFinder::getCodeCave(uint32_t realAddr, uint32_t hookAddr, uint32_t *cave)
  {
    if (cave == NULL)
      return false;

    if (this->findCaveInCache(realAddr, hookAddr, cave))
      return true;

    // We can't free this memory while application still run
    CaveInfo *caveInfo = new CaveInfo();

    if (!this->allocateCave(realAddr, hookAddr, caveInfo)) {
      // Failed to allocate memory
      return false;
    }

    this->_caves.push_front(caveInfo);
    return this->findCaveInCache(realAddr, hookAddr, cave);
  }

  bool CodeCaveFinder::allocateCave(uint32_t realAddr, uint32_t hookAddr, CaveInfo *сaveInfo)
  {
    uint64_t middle = ((uint64_t)realAddr + (uint64_t)hookAddr) / 2;

    uint32_t minAddr;
    uint32_t maxAddr;
    if (realAddr > hookAddr) {
      maxAddr = realAddr;
      minAddr = hookAddr;
    } else {
      maxAddr = hookAddr;
      minAddr = realAddr;
    }

    middle = middle - middle % this->_pageSize;

    uint32_t currentAddr = (uint32_t)middle;
    MEMORY_BASIC_INFORMATION memoryInfo;

    while (true) {
      if (((int64_t)currentAddr - (int64_t)minAddr) > (int64_t)INT_MAX)
        break;

      if (currentAddr > (uint32_t)this->_maximumApplicationAddress)
        break;

      VirtualQuery((LPCVOID)currentAddr, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

      if (memoryInfo.State == MEM_FREE) {
        LPVOID cave = VirtualAlloc((LPVOID)currentAddr, this->_pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (cave != NULL) {
          (*сaveInfo).startAddress = (uint32_t)cave;
          (*сaveInfo).currentAddress = (uint32_t)cave;
          (*сaveInfo).totalSize = this->_pageSize;
          (*сaveInfo).unusedSize = this->_pageSize;
          return true;
        }
      }

      currentAddr += this->_pageSize;
    }

    currentAddr = (uint32_t)middle - this->_pageSize;

    while (true) {
      if (((int64_t)minAddr - (int64_t)currentAddr) > (int64_t)INT_MAX)
        break;

      if (currentAddr < (uint32_t)this->_minimumApplicationAddress)
        break;

      VirtualQuery((LPCVOID)currentAddr, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

      if (memoryInfo.State == MEM_FREE) {
        LPVOID cave = VirtualAlloc((LPVOID)currentAddr,  this->_pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (cave != NULL) {
          (*сaveInfo).startAddress = (uint32_t)cave;
          (*сaveInfo).currentAddress = (uint32_t)cave;
          (*сaveInfo).totalSize = this->_pageSize;
          (*сaveInfo).unusedSize = this->_pageSize;
          return true;
        }
      }

      currentAddr -= this->_pageSize;
    }

    return false;
  }

  bool CodeCaveFinder::findCaveInCache(uint32_t realAddr, uint32_t hookAddr, uint32_t *сave)
  {
    if (сave == NULL)
      return false;

    if (this->_caves.size() <= 0)
      return false;

    for (auto iter = this->_caves.begin(); iter != this->_caves.end(); ++iter) {
      if ((*iter)->currentAddress == NULL || (*iter)->unusedSize < CAVE_SIZE)
        continue;

      int64_t current = (*iter)->currentAddress;
      int64_t realDiff = current > realAddr
        ? current - (int64_t)realAddr
        : (int64_t)realAddr - current;

      if (realDiff > (int64_t)INT_MAX)
        continue;

      int64_t hookDiff = current > hookAddr
        ? current - (int64_t)hookAddr
        : (int64_t)hookAddr - current;

      if (hookDiff > (int64_t)INT_MAX)
        continue;

      *сave = (*iter)->currentAddress;
      (*iter)->currentAddress += CAVE_SIZE;
      (*iter)->unusedSize -= CAVE_SIZE;
      return true;
    }

    return false;
  }

}

#endif