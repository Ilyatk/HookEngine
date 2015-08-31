#ifndef _WIN64
#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>
#include <iostream>

#pragma optimize("", off)
int __stdcall targetFunction2(int arg)
{
  return arg + 42;
}

int __stdcall targetFunction3(int arg)
{
  return arg + 32;
}

void __stdcall targetFunctionVoidPointer(int* arg)
{
  if (!arg)
    return;

  *arg = 42;
}

void __stdcall targetFunctionVoidReferene(int& arg)
{
  arg = 42;
}

int __stdcall targetDeleteReceiver(int arg)
{
  return 43 + arg;
}

int __stdcall targetDeleteReceiver2(int arg)
{
  return 44 + arg;
}

int __stdcall templateHookArg2(int arg, int arg1)
{
  return arg + arg1;
}

int __stdcall templateHookArg10(
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
  return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10;
}

int __stdcall targetFunction0()
{
  return 42;
}

int voidtargetFunction0Arg = 0;
void __stdcall voidtargetFunction0()
{
  voidtargetFunction0Arg += 42;
}


enum SomeResultAntached {
  Val1 = 1,
  Val2 = 2,
  Val3 = 3,
  Val4 = 4,
  Val5 = 5
};

class ThisCallHookTest1 {
public:
  enum SomeResult {
    Val1 = 1,
    Val2 = 2,
    Val3 = 3,
    Val4 = 4,
    Val5 = 5
  };

  ThisCallHookTest1(){}
  ~ThisCallHookTest1(){}

  int test() 
  {
    return 142;
  }

  int test2()
  {
    return 52;
  }

  int test3()
  {
    return 62;
  }

  void test4(int* a, SomeResult b) {
    std::cout << __FUNCTION__ << " " << this << " " << a << " " << b << std::endl;
  }

  void test5(int* a, SomeResultAntached b) {
    std::cout << __FUNCTION__ << " " << this << " " << a << " " << b << std::endl;
  }
};

#pragma optimize("", on)

TEST(TemplateHook, hookWithTemplate2)
{
  ASSERT_EQ(52, targetFunction2(10));

  uint32_t targetAddress = (uint32_t)targetFunction2;
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int>(targetAddress);

  hook->assignHook([hook](int arg1) -> int {
    int result = hook->original(arg1);
    return result + 20;
  });

  int afterHookresult = targetFunction2(10);
  ASSERT_EQ(72, afterHookresult);
}

TEST(TemplateHook, hookWithTemplate3)
{
  ASSERT_EQ(42, targetFunction3(10));

  uint32_t targetAddress = (uint32_t)targetFunction3;
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int>(targetAddress);

  hook->assignHook([hook](int arg1) -> int {
    int result = hook->original(arg1);
    return result + 10;
  });

  int afterHookresult = targetFunction3(10);
  ASSERT_EQ(52, afterHookresult);
}

TEST(TemplateHook, hookWithTemplateVoidPointer)
{
  int something = 0;
  targetFunctionVoidPointer(&something);
  ASSERT_EQ(42, something);

  auto hook = HookEngine::createHook<HookEngine::Stdcall, void, int*>((uint32_t)targetFunctionVoidPointer);
  hook->assignHook([hook](int* arg1) {
    if (!arg1)
      return;

    hook->original(arg1);
    *arg1 = *arg1 + 10;
  });

  targetFunctionVoidPointer(&something);
  ASSERT_EQ(52, something);
}

TEST(TemplateHook, hookWithTemplateVoidReference)
{
  int something = 0;
  targetFunctionVoidReferene(something);
  ASSERT_EQ(42, something);

  auto hook = HookEngine::createHook<HookEngine::Stdcall, void, int&>((uint32_t)targetFunctionVoidReferene);
  hook->assignHook([hook](int& arg1) {
    hook->original(arg1);
    arg1 = arg1 + 40;
  });

  targetFunctionVoidReferene(something);
  ASSERT_EQ(82, something);
}

#pragma optimize("", off)

TEST(TemplateHook, deleteReceiver)
{
  ASSERT_EQ(63, targetDeleteReceiver(20));
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int>((uint32_t)targetDeleteReceiver);
  hook->assignHook([hook](int arg1) -> int {
    int result = hook->original(arg1);
    return result + 30;
  });

  int afterHookresult = targetDeleteReceiver(20);
  ASSERT_EQ(93, afterHookresult);

  delete hook;

  int afterDeleteResult = targetDeleteReceiver(20);
  ASSERT_EQ(63, afterDeleteResult);

  auto hook2 = HookEngine::createHook<HookEngine::Stdcall, int, int>((uint32_t)targetDeleteReceiver);
  hook2->assignHook([hook2](int arg1) -> int {
    int result = hook2->original(arg1);
    return result + 40;
  });

  int afterHookresult2 = targetDeleteReceiver(20);
  ASSERT_EQ(103, afterHookresult2);
}

TEST(TemplateHook, deleteReceiver2)
{
  ASSERT_EQ(64, targetDeleteReceiver2(20));
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int>((uint32_t)targetDeleteReceiver2);
  hook->assignHook([hook](int arg1) -> int {
    int result = hook->original(arg1);
    return result + 30;
  });

  int afterHookresult = targetDeleteReceiver2(20);
  ASSERT_EQ(94, afterHookresult);

  hook->assignHook(nullptr); 

  int afterDeleteResult = targetDeleteReceiver2(20);
  ASSERT_EQ(64, afterDeleteResult);
}

#pragma optimize("", on)

TEST(TemplateHook, templateHookArg2)
{
  ASSERT_EQ(42, templateHookArg2(20, 22));

  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int, int>((uint32_t)templateHookArg2);
  hook->assignHook([hook](int arg1, int arg2) -> int {
    int result = hook->original(arg1, arg2);
    return result + arg1 + arg2;
  });

  int afterHookresult = templateHookArg2(20, 22);
  ASSERT_EQ(84, afterHookresult);
}

TEST(TemplateHook, templateHookArg10)
{
  ASSERT_EQ(55, templateHookArg10(1,2,3,4,5,6,7,8,9,10));

  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int, int, int, int, int, int, int, int, int, int>(
    (uint32_t)templateHookArg10);

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
    int a10
    ) -> int {
      int result = hook->original(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
      return result + 20;
  });

  int afterHookresult = templateHookArg10(1,2,3,4,5,6,7,8,9,10);
  ASSERT_EQ(75, afterHookresult);
}

TEST(TemplateHook, hookWithTemplateArg0)
{
  ASSERT_EQ(42, targetFunction0());

  uint32_t targetAddress = (uint32_t)targetFunction0;
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int>(targetAddress);

  hook->assignHook([hook]() -> int {
    int result = hook->original();
    return result + 30;
  });

  ASSERT_EQ(72, targetFunction0());
}

TEST(TemplateHook, hookWithVoidTemplateArg0)
{
  voidtargetFunction0Arg = 0;
  voidtargetFunction0();
  ASSERT_EQ(42, voidtargetFunction0Arg);

  uint32_t targetAddress = (uint32_t)voidtargetFunction0;
  auto hook = HookEngine::createHook<HookEngine::Stdcall, void>(targetAddress);

  hook->assignHook([hook]() {
    hook->original();
    voidtargetFunction0Arg += 30;
  });

  voidtargetFunction0Arg = 0;
  voidtargetFunction0();
  ASSERT_EQ(72, voidtargetFunction0Arg);
}

TEST(TemplateHook_ThisCall, hookWithVoidTemplateArg0)
{
  ThisCallHookTest1 obj;
  ASSERT_EQ(142, obj.test());

  uint32_t target = HookEngine::ClassFuntionPointerToUint32(&ThisCallHookTest1::test);
  auto hook = HookEngine::createHook<HookEngine::Thiscall, int, ThisCallHookTest1*>(target);
  hook->assignHook([hook](ThisCallHookTest1* sender) -> int {
    return 20 + hook->original(sender);
  });

  ASSERT_EQ(162, obj.test());
}

TEST(TemplateHook_ThisCall, hookWithVoidTemplateArg0WithoutHook)
{
  ThisCallHookTest1 obj;
  ASSERT_EQ(52, obj.test2());

  uint32_t target = HookEngine::ClassFuntionPointerToUint32(&ThisCallHookTest1::test2);
  auto hook = HookEngine::createHook<HookEngine::Thiscall, int, ThisCallHookTest1*>(target);
  ASSERT_EQ(52, obj.test2());
}

TEST(TemplateHook_ThisCall, hookWithVoidTemplateArg0WithoutSender)
{
  ThisCallHookTest1 obj;
  ASSERT_EQ(62, obj.test3());

  uint32_t target = HookEngine::ClassFuntionPointerToUint32(&ThisCallHookTest1::test3);
  auto hook = HookEngine::createHook<HookEngine::Stdcall, int>(target);
  hook->assignHook([hook]() -> int {
    return 72;
  });

  ASSERT_EQ(72, obj.test3());
}


TEST(TemplateHook_ThisCall, ProfilerCrashTest)
{
  ThisCallHookTest1 obj;
  int a = 43;
  ThisCallHookTest1::SomeResult b = ThisCallHookTest1::Val3;

  std::cout << std::endl << "ThisCallHookTest1::SomeResult size: " <<  sizeof(b) << " actual type size : " << sizeof(ThisCallHookTest1::SomeResult) <<std::endl;

  uint32_t target = HookEngine::ClassFuntionPointerToUint32(&ThisCallHookTest1::test4);

  auto hook = HookEngine::createHook<HookEngine::Thiscall, void, ThisCallHookTest1*, int*, ThisCallHookTest1::SomeResult>(target);
  hook->assignHook([hook](ThisCallHookTest1* _this, int* _a, ThisCallHookTest1::SomeResult _b) {
    std::cout << "!!!! " << _this << " " << _a << " " << _b << std::endl;
    hook->original(_this, _a, _b);
  });

  obj.test4(&a, b);
}



TEST(TemplateHook_ThisCall, ProfilerCrashTest2)
{
  ThisCallHookTest1 obj;
  int a = 43;
  SomeResultAntached b = SomeResultAntached::Val4;

  std::cout << std::endl << "SomeResultAntached size: " <<  sizeof(b) << " actual type size : " << sizeof(SomeResultAntached) << std::endl;

  uint32_t target = HookEngine::ClassFuntionPointerToUint32(&ThisCallHookTest1::test5);

  auto hook = HookEngine::createHook<HookEngine::Thiscall, void, ThisCallHookTest1*, int*, SomeResultAntached>(target);
  hook->assignHook([hook](ThisCallHookTest1* _this, int* _a, SomeResultAntached _b) {
    std::cout << "!!!! " << _this << " " << _a << " " << _b << std::endl;
    hook->original(_this, _a, _b);
  });

  obj.test5(&a, b);
}


#endif