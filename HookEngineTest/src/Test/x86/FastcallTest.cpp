#ifndef _WIN64
#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>

#pragma optimize("", off)
int __fastcall targetFastcallFunc1(int a1, int a2, int a3)
{
  return 42 + a1 + a2 + a3;
}
#pragma optimize("", on)

TEST(Fastcall, Sample)
{
  ASSERT_EQ(102, targetFastcallFunc1(10, 20, 30));

  auto hook = HookEngine::createHook<HookEngine::Fastcall, int, int, int, int>((uint32_t)targetFastcallFunc1);
  hook->assignHook([hook](int a1, int a2, int a3) -> int {
    return 10 + hook->original(a1, a2, a3);
  });

  ASSERT_EQ(112, targetFastcallFunc1(10, 20, 30));
}

#pragma optimize("", off)
int __fastcall targetFastcallFunc2(int a1, int a2)
{
  return 42 + a1 + a2;
}
#pragma optimize("", on)

TEST(Fastcall, Sample2)
{
  ASSERT_EQ(72, targetFastcallFunc2(10, 20));

  auto hook = HookEngine::createHook<HookEngine::Fastcall, int, int, int>((uint32_t)targetFastcallFunc2);
  hook->assignHook([hook](int a1, int a2) -> int {
    return 10 + hook->original(a1, a2);
  });

  ASSERT_EQ(82, targetFastcallFunc2(10, 20));
}

#pragma optimize("", off)
int __fastcall targetFastcallFunc3(
  int a1,
  int a2,
  int a3,
  int a4,
  int a5,
  int a6,
  int a7,
  int a8,
  int a9,
  int a10)
{
  return 10 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10;
}
#pragma optimize("", on)

TEST(Fastcall, Sample3)
{
  ASSERT_EQ(65, targetFastcallFunc3(1,2,3,4,5,6,7,8,9,10));

  auto hook = HookEngine::createHook<HookEngine::Fastcall, int, int, int, int, int, int, int, int, int, int, int>(
    (uint32_t)targetFastcallFunc3);

  hook->assignHook([hook](
    int a1,
    int a2,
    int a3,
    int a4,
    int a5,
    int a6,
    int a7,
    int a8,
    int a9,
    int a10) -> int {
      int result = hook->original(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
      return result + 20;
  });

  ASSERT_EQ(85, targetFastcallFunc3(1,2,3,4,5,6,7,8,9,10));
}

#pragma optimize("", off)
int __fastcall targetFastcallFunc4(int a1)
{
  return 42 + a1;
}
#pragma optimize("", on)

TEST(Fastcall, Sample4)
{
  ASSERT_EQ(72, targetFastcallFunc4(30));

  auto hook = HookEngine::createHook<HookEngine::Fastcall, int, int>((uint32_t)targetFastcallFunc4);
  hook->assignHook([hook](int a1) -> int {
    return 10 + hook->original(a1);
  });

  ASSERT_EQ(82, targetFastcallFunc4(30));
}
#endif