#ifdef _WIN64

#include <gtest/gtest.h>

#include <HookEngine/HookEngine.h>

#pragma optimize("", off)
int targetFunction(double a, double b, int c)
{
  double d = sqrt(b*b - 4 * a*c);
  double result = (-b - d) / (2 * a);

  return static_cast<int>(result);
}

int targetFunctionForClassHook(double a, double b, int c)
{
  if (c > 9999)
    return -9999;

  return 2;

}

#pragma optimize("", on)

TEST(Sample, simpleCallTargetFunction)
{
  ASSERT_EQ(2, targetFunction(1, -4, 4));
}

#pragma optimize("", off)
volatile int globalCounter;
HookEngine::HookInfo *info = NULL;

typedef int(__stdcall *fnOriginal)(double, double, int);

int hook(double a, double b, int c)
{
  globalCounter = static_cast<int>(a + b + c);
  int result = ((fnOriginal)(info->afterPtr()))(a, b, c);
  return result;
}
#pragma optimize("", on)

TEST(Sample, hookStaticFunction)
{
  globalCounter = 999;
  ASSERT_EQ(999, globalCounter);

  ASSERT_TRUE(HookEngine::instance()->installHook(
    reinterpret_cast<uintptr_t>(targetFunction),
    reinterpret_cast<uintptr_t>(hook),
    &info));

  ASSERT_TRUE(info != NULL);

  ASSERT_EQ(2, targetFunction(1, -4, 4));
  ASSERT_EQ(1, globalCounter);
}

#endif