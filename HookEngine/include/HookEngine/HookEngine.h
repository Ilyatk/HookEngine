#pragma once

#ifdef _WIN64
  #include <HookEngine/private/x64/HookEngine64.h>
#else
  #include <HookEngine/private/x86/HookEngine86.h>
#endif

#include <HookEngine/Hook.h>
#include <HookEngine/AutoLink.h>

namespace HookEngine {
  HookEngine* instance();
}