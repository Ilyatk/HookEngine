// From StdcallFasthook example we know how HookEngine::fasthook use second argument.
// Now let's compare __fastcall and __thicall function:
// stuct Test {
//	int f1(int a1, int a2);
// };
//	
// int __fastcall f2(LPVOID _ecx, int _edx, int a1, int a2);
//  
// Thiscall function use ECX register to pass THIS pointer; all other arguments are pased right to left.
// Thus thiscall function is same as fastcall function with unused second argument. 

#include <HookEngine/HookEngine.h>
#include <Windows.h>

// Turning off optimization. Otherwise such simple function will be replaced with inline call.
#pragma optimize("", off)

class TargetClass 
{
public:
  TargetClass(){};
  ~TargetClass(){};

  int showMessage(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
  {
    return MessageBoxA(hWnd, lpText, lpCaption, uType);
  }
};
#pragma optimize("", on)

// With fastcall function we can call class member function from raw pointer and pass THIS pointer thought first argument.
// Second argument can be anything because it's unused.
typedef int (__fastcall *fnShowMessage)(TargetClass* target, int unused, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

// This function is almost same as original but calling convention is __fastcall now. Also It has two additional arguments. 
// First argument is THIS pointer. In second argument is original function address. 
int __fastcall ShowMessageHook(TargetClass* target, fnShowMessage original, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  // We can't call target->showMessage(...) because we will stuck in infinity loop.

  return original(target, 0, hWnd, "The Matrix has you...", lpCaption, uType);
}


void main()
{
  TargetClass target;

  target.showMessage(0, "Not hooked yet", "Warning", MB_OK);

  // Some trick to cast member function pointer to uint32_t
  uint32_t original = HookEngine::ClassFuntionPointerToUint32(&TargetClass::showMessage);
  HookEngine::HookEngine::instance()->fasthook(original, (uint32_t)ShowMessageHook);

  target.showMessage(0, "Not hooked yet", "Warning", MB_OK);
}
