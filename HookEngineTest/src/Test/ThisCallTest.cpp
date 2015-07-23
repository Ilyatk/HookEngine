#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>

#include <memory>

#pragma optimize("", off)
class SomeSimpleClass {
public:
  SomeSimpleClass() : a(10), g(15) {};
  ~SomeSimpleClass() {};

  int targetFunction(double a, double b, int c) 
  {
    double d = sqrt(b*b - 4*a*c);
    double result = (-b - d) / (2 * a);

    return static_cast<int>(result);
  }

  int a;
  int g;
};
#pragma optimize("", on)

TEST(ThisCall, sample)
{
  std::unique_ptr<SomeSimpleClass> target(new SomeSimpleClass);
  int result = target->targetFunction(1,-4,4);
  ASSERT_EQ(2, result);
}

// Попробуем эмулировать хук через 
int globalCounter2 = 0;

HookEngine::HookInfo *info2;

typedef int (__stdcall *fnOriginal)(double,double,int);

int __fastcall fastCallhook(SomeSimpleClass *sender, int fake, double a, double b, int c)
{
  globalCounter2 = static_cast<int>(a + b + c) + sender->a;
  sender->g = 1999;

  // BTW it's old way to call __thiscall function. 
  fnOriginal original = (fnOriginal)(info2->afterPtr()); 
  __asm {
    mov ecx, sender;
  }

  int result = (original)(a,b,c);
  return result;
}

TEST(ThisCall, sampleHook)
{
  globalCounter2 = 999;

  ASSERT_EQ(999, globalCounter2);

  std::unique_ptr<SomeSimpleClass> target(new SomeSimpleClass);
  int result = target->targetFunction(1,-4,4);
  ASSERT_EQ(2, result);

  uint32_t originalHook = HookEngine::ClassFuntionPointerToUint32(&SomeSimpleClass::targetFunction);

  info2 = NULL;
  ASSERT_TRUE(HookEngine::HookEngine::instance()->installHook(
    originalHook, reinterpret_cast<uint32_t>(fastCallhook), &info2));
  ASSERT_TRUE(info2 != NULL);

  ASSERT_EQ(15, target->g);
  result = target->targetFunction(1,-4,4);
  ASSERT_EQ(2, result);
  ASSERT_EQ(1999, target->g);
}

