
#include <HookEngine/HookEngine.h>
#include <Windows.h>
#include <iostream>
#include <string>

// Turning off optimization. Otherwise such simple function will be replaced with inline call.
#pragma optimize("", off)

int __stdcall targetStdcall(int a1, int a2) 
{
  return a1 + a2;
}

int __cdecl targetCdecl(const char *msg, int count) 
{
  std::cout << "Cdecl message " << msg << " Count = " << count << std::endl;
  return 10 * count;
}

void __fastcall targetFastcall(int a1, int a2, int a3, int& result)
{
  result = a1 * 100 + a2 * 10 + a3;
}

class BankStorage 
{
public:
  BankStorage(int value)
    : _money(value)
  {
  };

  ~BankStorage(){};

  void addMoney(int money)
  {
    this->_money += money;
    //Bank's money has been increased by 10. There are 110$ now.
    std::cout << "Bank's money has been increased by " 
      << money
      << "$. There are "
      << this->_money
      << "$ now."
      << std::endl;
  }

private:
  int _money;
};
#pragma optimize("", on)

void main()
{
  // ------------------------ Stdcall example ----------------------
  std::cout << "Stdcall function examples:" << std::endl << std::endl;
  std::cout << "before hook                       targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // Create hook.
  auto targetStdcallHook = HookEngine::createHook<
    HookEngine::Stdcall, // target function calling convention
    int, // return type
    int, // first argument type
    int  // second argument type
  >((uint32_t)targetStdcall);

  std::cout << "It's still same function          targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // Set hook function.
  targetStdcallHook->assignHook([targetStdcallHook](int b1, int b2) -> int {
    return 1000 + targetStdcallHook->original(b1, b2);
  });

  std::cout << "Hook changed result               targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // Remove hook function and get original.
  targetStdcallHook->assignHook(0);

  std::cout << "Once again original result        targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // Set different hook function.
  targetStdcallHook->assignHook([targetStdcallHook](int b1, int b2) -> int {
    return 2000;
  });

  std::cout << "Other hook result                 targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // Let's delete hook
  delete targetStdcallHook;
  std::cout << "Once again original result        targetStdcall(10, 100) = " << targetStdcall(10, 100) << std::endl;

  // ------------------------ Cdecl example ----------------------
  std::cout << std::endl << std::endl << "Cdecl function example:" << std::endl << std::endl;

  std::cout << "original    targetCdecl(\"Hello\", 42):" << std::endl;
  int res = targetCdecl("Hello", 42);
  std::cout << "result  = "<< res << std::endl << std::endl;

  // Create hook.
  auto targetCdeclHook = HookEngine::createHook<
    HookEngine::Cdecl, // target function calling convention
    int,					// return type
    const char *, // first argument type
    int						// second argument type
  >((uint32_t)targetCdecl);

  targetCdeclHook->assignHook([targetCdeclHook](const char *msg, int count) -> int {
    return 10 * targetCdeclHook->original("Hooked successfully.", count);
  });

  std::cout << "hooked     targetCdecl(\"Hello\", 42):" << std::endl;
  res = targetCdecl("Hello", 42);
  std::cout << "result  = "<< res << std::endl << std::endl;

  // ------------------------ Fastcall example ----------------------
  std::cout << std::endl << std::endl << "Fastcall function example:" << std::endl << std::endl;

  //void __fastcall targetFastcall(int a1, int a2, int a3, int& result)
  int fast = 0;
  targetFastcall(5, 3, 1, fast);

  std::cout << "before hook   targetFastcall(5,3,1) = " << fast << std::endl;
  auto targetFastcallHook = HookEngine::createHook<
    HookEngine::Fastcall, // target function calling convention
    void,					// return type
    int, // first argument type
    int, // second argument type
    int, // third argument type
    int& // fourth argument type
  >((uint32_t)targetFastcall);

  targetFastcallHook->assignHook([targetFastcallHook](int a1, int a2, int a3, int& result) {
    int orig = 0;
    targetFastcallHook->original(a1, a2, a3, orig);
    orig += 1000;
    result = orig;
  });

  targetFastcall(5, 3, 1, fast);
  std::cout << "after hook    targetFastcall(5,3,1) = " << fast << std::endl;

  // ------------------------ Thiscall example ----------------------
  std::cout << std::endl << std::endl << "Thiscall function example:" << std::endl << std::endl;

  BankStorage storage(100);
  std::cout << "Increase bank money by 10 before hook:" << std::endl;
  storage.addMoney(10);

  auto targetThiscallHook = HookEngine::createHook<
    HookEngine::Thiscall, // target function calling convention
    void,		// return type
    void *, // THIS pointer type
    int     // first argument type
  >(HookEngine::ClassFuntionPointerToUint32(&BankStorage::addMoney));

  targetThiscallHook->assignHook([targetThiscallHook](void* _THIS, int money) {
    targetThiscallHook->original(_THIS, money * 10000);
  });

  std::cout << "Increase bank money by 10 after hook:" << std::endl;
  storage.addMoney(10);
}