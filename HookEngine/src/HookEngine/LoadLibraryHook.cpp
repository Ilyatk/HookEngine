#include <HookEngine/LoadLibraryHook.h>
#include <HookEngine/HookEngine.h>

#include <iostream>

namespace HookEngine {

  LoadLibraryHook::LoadLibraryHook()
  {
  }

  LoadLibraryHook::~LoadLibraryHook()
  {
  }

  void LoadLibraryHook::init()
  {
    HMODULE kernel32Handle = GetModuleHandleA("kernel32.dll");

    auto hookLoadLibraryA = createHook<Stdcall, HMODULE, LPCSTR>(kernel32Handle, "LoadLibraryA");

    hookLoadLibraryA->assignHook([this, hookLoadLibraryA] (LPCSTR name) -> HMODULE {
      HMODULE module = hookLoadLibraryA->original(name);
      DWORD error = GetLastError();
      this->onLoadModule(module);
      SetLastError(error);
      return module;
    });

    auto hookLoadLibraryW = createHook<Stdcall, HMODULE, LPCWSTR>(kernel32Handle, "LoadLibraryW");

    hookLoadLibraryW->assignHook([this, hookLoadLibraryW] (LPCWSTR name) -> HMODULE {
      HMODULE module = hookLoadLibraryW->original(name);
      DWORD error = GetLastError();
      this->onLoadModule(module);
      SetLastError(error);
      return module;
    });

    auto hookLoadLibraryExA = createHook<Stdcall, HMODULE, LPCSTR,HANDLE ,DWORD>(kernel32Handle, "LoadLibraryExA");

    hookLoadLibraryExA->assignHook([this, hookLoadLibraryExA] (LPCSTR name, HANDLE h, DWORD f) -> HMODULE {
      HMODULE module = hookLoadLibraryExA->original(name,h,f);
      DWORD error = GetLastError();
      this->onLoadModule(module);
      SetLastError(error);
      return module;
    });

    auto hookLoadLibraryExW = createHook<Stdcall, HMODULE, LPCWSTR,HANDLE ,DWORD>(kernel32Handle, "LoadLibraryExW");

    hookLoadLibraryExW->assignHook([this, hookLoadLibraryExW] (LPCWSTR name, HANDLE h, DWORD f) -> HMODULE {
      HMODULE module = hookLoadLibraryExW->original(name,h,f);
      DWORD error = GetLastError();
      this->onLoadModule(module);
      SetLastError(error);
      return module;
    });

    auto HookFreeModule = createHook<Stdcall, void, HMODULE>(kernel32Handle, "FreeLibrary");
    HookFreeModule->assignHook([this, HookFreeModule] (HMODULE module) {
      HookFreeModule->original(module);
      DWORD error = GetLastError();
      this->onUnloadModule(module);
      SetLastError(error);
    });

    auto HookFreeModuleAndExit = createHook<Stdcall, void, HMODULE, DWORD>(kernel32Handle, "FreeLibraryAndExitThread");

    HookFreeModuleAndExit->assignHook([this, HookFreeModuleAndExit] (HMODULE module, DWORD exitCode) {
      HookFreeModuleAndExit->original(module, exitCode);
      DWORD error = GetLastError();
      this->onUnloadModule(module);
      SetLastError(error);
    });
  }

  void LoadLibraryHook::onLoadModule(HMODULE module)
  {
    HookEngine::instance()->moduleLoaded(module);
  }

  void LoadLibraryHook::onUnloadModule(HMODULE module)
  {
    HookEngine::instance()->moduleUnloaded(module);
  }

}