#ifdef _WIN64
#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>
#include <iostream>

#pragma optimize("", off)

int targetTemplate_Args5(int a1, int a2, int a3, int a4, int a5)
{
  return 10000 + a1 + a2 + a3 + a4 + a5;
}

int targetTemplate_Args4(int a1, int a2, int a3, int a4)
{
  return 10000 + a1 + a2 + a3 + a4;
}

int targetTemplate_Args3(int a1, int a2, int a3)
{
  return 10000 + a1 + a2 + a3;
}

int targetTemplate_Args2(int a1, int a2)
{
  return 10000 + a1 + a2;
}

int targetTemplate_Args1(int a1)
{
  return 10000 + a1;
}

int targetTemplate_Args0()
{
  return 10000;
}

int targetTemplate_MixArgs(int a1, float a2, int a3, float a4, int& intResult, float& floatResult)
{
  intResult = a1 + a3 + 1000;
  floatResult = a2 + a4 + 1000.0f;
  return 42;
}

#pragma optimize("", on)

TEST(TemplateHook64, hookWithTemplate_args5)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args5;

  auto hook = HookEngine::createHook<HookEngine::Win64, int, int, int, int, int, int>(target);
  hook->assignHook([hook](int a1, int a2, int a3, int a4, int a5) -> int {
    int result = hook->original(a1, a2, a3, a4, a5);
    return result + 10000;
  });

  int q = targetTemplate_Args5(5, 60, 700, 8000, 300000);
  ASSERT_EQ(328765, q);
}

TEST(TemplateHook64, hookWithTemplate_args4)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args4;

  auto hook = HookEngine::createHook<HookEngine::Win64, int, int, int, int, int>(target);
  hook->assignHook([hook](int a1, int a2, int a3, int a4) -> int {
    int result = hook->original(a1, a2, a3, a4);
    return result + 10000;
  });

  int q = targetTemplate_Args4(5, 60, 700, 8000);
  ASSERT_EQ(28765, q);
}

TEST(TemplateHook64, hookWithTemplate_args3)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args3;

  auto hook = HookEngine::createHook<HookEngine::Win64, int, int, int, int>(target);
  hook->assignHook([hook](int a1, int a2, int a3) -> int {
    int result = hook->original(a1, a2, a3);
    return result + 10000;
  });

  int q = targetTemplate_Args3(5, 60, 700);
  ASSERT_EQ(20765, q);
}

TEST(TemplateHook64, hookWithTemplate_args2)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args2;

  auto hook = HookEngine::createHook<HookEngine::Win64, int, int, int>(target);
  hook->assignHook([hook](int a1, int a2) -> int {
    int result = hook->original(a1, a2);
    return result + 10000;
  });

  int q = targetTemplate_Args2(5, 60);
  ASSERT_EQ(20065, q);
}

TEST(TemplateHook64, hookWithTemplate_args1)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args1;

  auto hook = HookEngine::createHook<HookEngine::Win64, int, int>(target);
  hook->assignHook([hook](int a1) -> int {
    int result = hook->original(a1);
    return result + 10000;
  });

  int q = targetTemplate_Args1(5);
  ASSERT_EQ(20005, q);
}

TEST(TemplateHook64, hookWithTemplate_args0)
{
  uintptr_t target = (uintptr_t)targetTemplate_Args0;

  auto hook = HookEngine::createHook<HookEngine::Win64, int>(target);
  hook->assignHook([hook]() -> int {
    int result = hook->original();
    return result + 10000;
  });

  int q = targetTemplate_Args0();
  ASSERT_EQ(20000, q);
}

TEST(TemplateHook64, hookWithTemplate_mixArgs)
{
  int intRes = 0;
  float floatRes = 0.0f;
  
  int res = targetTemplate_MixArgs(700, 700, 30, 30, intRes, floatRes);
  ASSERT_EQ(42, res);
  ASSERT_EQ(1730, intRes);
  ASSERT_FLOAT_EQ(1730.0f, floatRes);

  intRes = 0;
  floatRes = 0.0f;
  res = 0;

  uintptr_t target = (uintptr_t)targetTemplate_MixArgs;
  auto hook = HookEngine::createHook<HookEngine::Win64, int, int, float, int, float, int&, float&>(target);
  hook->assignHook([hook](int a1, float a2, int a3, float a4, int& intResult, float& floatResult) -> int {
    int res = hook->original(a1, a2, a3, a4, intResult, floatResult);

    intResult += 10000;
    floatResult += 10000.0f;

    return res + 1;
  });

  res = targetTemplate_MixArgs(700, 700, 30, 30, intRes, floatRes);
  ASSERT_EQ(43, res);
  ASSERT_EQ(11730, intRes);
  ASSERT_FLOAT_EQ(11730.0f, floatRes);
}

#endif