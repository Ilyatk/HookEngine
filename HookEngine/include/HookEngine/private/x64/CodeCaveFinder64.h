#pragma once

#ifdef _WIN64

#include <stdint.h>

#include <Windows.h>
#include <list>

#define CAVE_SIZE 0x100

namespace HookEngine {

  //typedef struct CAVEINFO
  //{
  //  uintptr_t startAddress;
  //  uintptr_t currentAddress;
  //  uintptr_t totalSize;
  //  uintptr_t unusedSize;

  //  CAVEINFO()
  //  {
  //    startAddress = NULL;
  //    currentAddress = NULL;
  //    totalSize = 0;
  //    unusedSize = 0;
  //  }

  //  CAVEINFO(uintptr_t startAddress, uintptr_t currentAddress, uintptr_t totalSize, uintptr_t unusedSize)
  //  {
  //    startAddress = startAddress;
  //    currentAddress = currentAddress;
  //    totalSize = totalSize;
  //    unusedSize = unusedSize;
  //  }

  //  ~CAVEINFO()
  //  {
  //  }
  //} CaveInfo;

  class CaveInfo;

  class CodeCaveFinder
  {
  public:
    CodeCaveFinder();
    ~CodeCaveFinder();

    bool getCodeCave(uintptr_t realAddr, uintptr_t *cave);

  private:
    std::list<CaveInfo *> _caves;
    DWORD _pageSize;
    uintptr_t _minimumApplicationAddress;
    uintptr_t _maximumApplicationAddress;

    bool allocateCave(uintptr_t realAddr, CaveInfo **caveInfo);
    bool getFromCache(uintptr_t realAddr, CaveInfo **caveInfo);
    uintptr_t consume(CaveInfo *caveInfo, size_t caveSize);
  };
}

#endif