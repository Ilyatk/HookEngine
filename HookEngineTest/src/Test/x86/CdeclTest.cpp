#ifndef _WIN64
#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>

#pragma optimize("", off)
int __cdecl targetCdeclFunc1(int a1, int a2, int a3)
{
  return 42 + a1 + a2 + a3;
}
#pragma optimize("", on)

TEST(CdeclTest, Sample)
{
  ASSERT_EQ(102, targetCdeclFunc1(10, 20, 30));

  auto hook = HookEngine::createHook<HookEngine::Cdecl, int, int, int, int>((uint32_t)targetCdeclFunc1);
  hook->assignHook([hook](int a1, int a2, int a3) -> int {
    return 40 + hook->original(a1, a2, a3);
  });

  ASSERT_EQ(142, targetCdeclFunc1(10, 20, 30));
}

#pragma optimize( "", off)
int __cdecl targetCdeclFunc2()
{
  return 142;
}
#pragma optimize( "", on)

TEST(CdeclTest, SampleNoArgs)
{
  ASSERT_EQ(142, targetCdeclFunc2());

  auto hook = HookEngine::createHook<HookEngine::Cdecl, int>((uint32_t)targetCdeclFunc2);
  hook->assignHook([hook]() -> int {
    return 40 + hook->original();
  });

  ASSERT_EQ(182, targetCdeclFunc2());
}

#endif