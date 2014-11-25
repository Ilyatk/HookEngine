#include <HookEngine/HookEngine.h>
#include <Windows.h>

typedef int (__stdcall *fnMessageBox)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

HookEngine::HookInfo* mesageBoxInfo = NULL;

// Hook function must have same prototype as target function.
int __stdcall MessageBoxAHook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  fnMessageBox originalMessageBox = (fnMessageBox)mesageBoxInfo->afterPtr();
  return originalMessageBox(hWnd, "You can't escape.", lpCaption, uType);
}

void main()
{
  MessageBoxA(0, "Not hooked yet", "Warning", MB_OK);

  HMODULE user32 = GetModuleHandleA("User32.dll");
  uint32_t original = (uint32_t)GetProcAddress(user32, "MessageBoxA");

  HookEngine::HookEngine::instance()->installHook(original, (uint32_t)MessageBoxAHook, &mesageBoxInfo);

  MessageBoxA(0, "Not hooked yet", "Warning", MB_OK);
}