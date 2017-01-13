#ifdef _WIN64

#include <gtest/gtest.h>

#include <windows.h>
#include <cstdint>

#include <HookEngine/HookEngine.h>

typedef BOOL(WINAPI *fnVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
static HookEngine::HookInfo* VirtualProtectInfo;

int VirtualProtectHook(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
  return ((fnVirtualProtect)VirtualProtectInfo->afterPtr())(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

TEST(Sample, hookVirtualProtect)
{
  uintptr_t original = (uintptr_t)GetProcAddress(::GetModuleHandle(L"Kernel32.dll"), "VirtualProtect");
  if (!original) {
    ASSERT_TRUE(false);
    return;
  }

  HookEngine::instance()->installHook(original, (uintptr_t)VirtualProtectHook, &VirtualProtectInfo);

  ::VirtualProtect(0, 0, 0, nullptr); // try call with fake arguments
  ASSERT_TRUE(true);
}

typedef int(WINAPI *fnMessageBoxW)(HWND, LPCTSTR, LPCTSTR, UINT);
static HookEngine::HookInfo* MessageBoxWHookInfo;

static bool test = false;

int MessageBoxWHook(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
  test = true;
  return ((fnMessageBoxW)MessageBoxWHookInfo->afterPtr())(hWnd, L"Hello from Hook!", L"Test Hooked", uType);
}

TEST(Sample, hookMessageBox)
{
  ::MessageBox(nullptr, L"Hello", L"Test", MB_OK);
  ASSERT_FALSE(test);

  uintptr_t original = (uintptr_t)GetProcAddress(::GetModuleHandle(L"User32.dll"), "MessageBoxW");
  if (!original) {
    ASSERT_TRUE(false);
    return;
  }

  HookEngine::instance()->installHook(original, (uintptr_t)MessageBoxWHook, &MessageBoxWHookInfo);

  ::MessageBox(nullptr, L"Hello", L"Test", MB_OK);
  ASSERT_TRUE(test);
}

#endif