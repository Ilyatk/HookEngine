#pragma once

#ifdef _WIN64

#include <stdint.h>

#include <Windows.h>
#include <list>

#define CAVE_SIZE 0x100

namespace HookEngine {

  class CaveInfo;

  class CodeCaveFinder
  {
  public:
    CodeCaveFinder();
    ~CodeCaveFinder();

    bool getCodeCave(uintptr_t realAddr, uintptr_t *cave);
    bool getCodeCave(uintptr_t *cave);

  private:
    std::list<CaveInfo *> _caves;
    std::list<CaveInfo *> _unattachedCaves;
    DWORD _pageSize;
    uintptr_t _minimumApplicationAddress;
    uintptr_t _maximumApplicationAddress;

    bool allocateCave(uintptr_t realAddr, CaveInfo **caveInfo);
    bool getFromCache(uintptr_t realAddr, CaveInfo **caveInfo);
    uintptr_t consume(CaveInfo *caveInfo, size_t caveSize);
  };
}

#endif