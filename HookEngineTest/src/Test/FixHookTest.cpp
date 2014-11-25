#include <HookEngine/HookEngine.h>

#include <gtest/gtest.h>

void ReloadLibrary() 
{
  HMODULE h = 0;
  while ((h = GetModuleHandleA("SampleDll2.dll")))
    FreeModule(h);

  h = LoadLibraryA("SampleDll2.dll");
}

typedef int(__stdcall *fnFixHook)(int,int,int);

#pragma optimize("", off)

TEST(fixHooks, fixHook)
{
  ReloadLibrary();

  HMODULE h2 = GetModuleHandleA("SampleDll2.dll");

  uint32_t target = (uint32_t)GetProcAddress(h2, "fixHookTarget");
  ASSERT_EQ(567, ((fnFixHook)target)(5, 6, 7));

  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int, int, int>(target);
  hook->assignHook([hook] (int a1, int a2, int a3) -> int {
    return 1000 + hook->original(a1,a2,a3);
  });

  ASSERT_EQ(1567, ((fnFixHook)target)(5, 6, 7));

  hook->makePersistent();

  ReloadLibrary();

  h2 = GetModuleHandleA("SampleDll2.dll");
  target = (uint32_t)GetProcAddress(h2, "fixHookTarget");
  ASSERT_EQ(1567, ((fnFixHook)target)(5,6,7));
}

#include <winternl.h>

typedef NTSTATUS(WINAPI *pfnZwQueryInformationProcess)(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength);


uint32_t GetAddressOfEntryPoint(HMODULE hmod)
{
  PIMAGE_DOS_HEADER pidh = (PIMAGE_DOS_HEADER)hmod;
  PIMAGE_NT_HEADERS pinth = (PIMAGE_NT_HEADERS)((PBYTE)hmod + pidh->e_lfanew);
  PVOID pvEntry = (PBYTE)hmod + pinth->OptionalHeader.AddressOfEntryPoint;

  return (uint32_t)pvEntry;
}

TEST(fixHooks, fixHook2)
{
  ReloadLibrary();

  HMODULE h2 = GetModuleHandleA("SampleDll2.dll");

  uint32_t entryPoint = GetAddressOfEntryPoint(h2);
  auto entryPointHook = HookEngine::createHook<HookEngine::Stdcall, BOOL, HMODULE, DWORD, LPVOID>(entryPoint);
  entryPointHook->assignHook([entryPointHook](HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL {

    return entryPointHook->original(hModule, ul_reason_for_call, lpReserved);
  });

  uint32_t target = (uint32_t)GetProcAddress(h2, "fixHookTarget2");

  ASSERT_EQ(5067, ((fnFixHook)target)(5, 6, 7));

  auto hook = HookEngine::createHook<HookEngine::Stdcall, int, int, int, int>(L"SampleDll2.dll", "fixHookTarget2");
  hook->assignHook([hook] (int a1, int a2, int a3) -> int {
    return 10000 + hook->original(a1,a2,a3);
  });

  ASSERT_EQ(15067, ((fnFixHook)target)(5, 6, 7));

  hook->makePersistent();

  ReloadLibrary();


  h2 = GetModuleHandleA("SampleDll2.dll");
  target = (uint32_t)GetProcAddress(h2, "fixHookTarget2");
  ASSERT_EQ(15067, ((fnFixHook)target)(5,6,7));
}

#pragma optimize("", on)