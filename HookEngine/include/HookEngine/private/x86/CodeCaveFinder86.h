#pragma once

#ifndef _WIN64

#include <stdint.h>

#include <Windows.h>
#include <list>

#define CAVE_SIZE 64

namespace HookEngine {

  class CaveInfo;
  class CodeCaveFinder
  {
  public:
    CodeCaveFinder();
    ~CodeCaveFinder();

    bool getCodeCave(uint32_t realAddr, uint32_t hookAddr, uint32_t *cave);
  private:
    std::list<CaveInfo *> _caves;
    DWORD _pageSize;
    LPVOID _minimumApplicationAddress;
    LPVOID _maximumApplicationAddress;

    bool findCaveInCache(uint32_t realAddr, uint32_t hookAddr, uint32_t *cave);
    bool allocateCave(uint32_t realAddr, uint32_t hookAddr, CaveInfo *caveInfo);
  };
}

#endif