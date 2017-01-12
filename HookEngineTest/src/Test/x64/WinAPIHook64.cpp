#ifdef _WIN64

#include <gtest/gtest.h>

#include <windows.h>
#include <cstdint>

#include <HookEngine/HookEngine.h>

using fnMessageBoxW = decltype(::MessageBoxW)*;

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
  ASSERT_TRUE(!test);

  uintptr_t original = (uintptr_t)GetProcAddress(::GetModuleHandle(L"User32.dll"), "MessageBoxW");
  if (!original) {
    ASSERT_TRUE(false);
    return;
  }

  HookEngine::instance()->installHook(original, (uintptr_t)MessageBoxWHook, &MessageBoxWHookInfo);
  HookEngine::instance()->installHook(original, (uintptr_t)MessageBoxWHook, &MessageBoxWHookInfo);

  ::MessageBox(nullptr, L"Hello", L"Test", MB_OK);

  ASSERT_TRUE(test);
}

#endif