#pragma once

#include <stdint.h>

#include <Windows.h>
#include <list>

#define CAVE_SIZE 64

namespace HookEngine {

  typedef struct CAVEINFO
  {
    uint32_t startAddress;
    uint32_t currentAddress;
    uint32_t totalSize;
    uint32_t unusedSize;

    CAVEINFO()
    {
      startAddress = NULL;
      currentAddress = NULL;
      totalSize = 0;
      unusedSize = 0;
    }

    CAVEINFO(uint32_t startAddress, uint32_t currentAddress, uint32_t totalSize, uint32_t unusedSize)
    {
      startAddress = startAddress;
      currentAddress = currentAddress;
      totalSize = totalSize;
      unusedSize = unusedSize;
    }

    ~CAVEINFO()
    {
    }
  } CaveInfo;

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