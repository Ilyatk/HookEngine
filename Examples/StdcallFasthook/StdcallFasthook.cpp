// In this example we use some trick with __fastcall funciton. 
// First two arguments are passed in ECX and EDX, all other arguments are passed right to left same as stdcall.
// Let's look at two function definition:
//	int __stdcall f1(int a1, int a2);
//	int __fastcall f2(int _ecx, int _edx, int a1, int a2);
//
// When you call this functions stack will be same. But in function f2 we also have two additional
// parameters _ecx and _edx that hook can use to pass some information.
// Function HookEngine::fasthook set into second argument(_edx) original function address.

// About using first argument see example ThisscallFasthook.

// More information about calling convention you can learn from http://msdn.microsoft.com/en-us/library/984x0h58.aspx

#include <HookEngine/HookEngine.h>
#include <Windows.h>

typedef int (__stdcall *fnMessageBox)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

// This function is almost same as original but calling convention is __fastcall now. Also It has two additional arguments. 
// In second argument we expect original function address. First argument is unused.
int __fastcall MessageBoxAHook(int unused, fnMessageBox original, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  return original(hWnd, "BAZINGA!", lpCaption, uType);
}

void main()
{
  MessageBoxA(0, "Not hooked yet", "Warning", MB_OK);

  HMODULE user32 = GetModuleHandleA("User32.dll");
  uint32_t original = (uint32_t)GetProcAddress(user32, "MessageBoxA");

  HookEngine::HookEngine::instance()->fasthook(original, (uint32_t)MessageBoxAHook);

  MessageBoxA(0, "Not hooked yet", "Warning", MB_OK);

}