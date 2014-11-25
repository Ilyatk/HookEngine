#pragma once

#include <Windows.h>

namespace HookEngine {

  class LoadLibraryHook
  {
  public:
    LoadLibraryHook();
    ~LoadLibraryHook();

    void init();

  private:
    void onLoadModule(HMODULE module);
    void onUnloadModule(HMODULE module);
  };
}
