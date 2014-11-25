#include <gtest/gtest.h>

#include <HookEngine/HookEngine.h>

void unloadTarget()
{
	HMODULE oldOne = GetModuleHandleA("SampleDll.dll");
	if (oldOne)
		FreeModule(oldOne);
}

HMODULE loadTarget()
{
	unloadTarget();
	return LoadLibraryA("SampleDll.dll");
}

HookEngine::HookInfo *stdcallInfo = NULL;

int __stdcall stdcallSampleFunctionHook(int x) {
	int result = ((int(__stdcall *)(int))stdcallInfo->afterPtr())(0);
	return result + x;
}

TEST(IncrimentalLinking, stdcall) 
{
	HMODULE dll = loadTarget(); 
	ASSERT_NE((HMODULE)0, dll);
	uint32_t original = (uint32_t)GetProcAddress(dll, "stdcallSampleFunction");
	ASSERT_NE(0, original);
	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(0));
	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(10));

	uint32_t target = (uint32_t)stdcallSampleFunctionHook;
	ASSERT_TRUE(HookEngine::HookEngine::instance()->installHook(original, target, &stdcallInfo));

	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(0));
	ASSERT_EQ(52, ((int(__stdcall *)(int))original)(10));
	unloadTarget();
}

// ---------------------------------------
HookEngine::HookInfo *cdeclInfo = NULL;

int __cdecl cdeclSampleFunctionHook(int x) {
	int result = ((int(__cdecl *)(int))cdeclInfo->afterPtr())(0);
	return result + x;
}

TEST(IncrimentalLinking, cdeclTest) 
{
	HMODULE dll = loadTarget(); 
	ASSERT_NE((HMODULE)0, dll);
	uint32_t original = (uint32_t)GetProcAddress(dll, "cdeclSampleFunction");
	ASSERT_NE(0, original);
	ASSERT_EQ(42, ((int(__cdecl *)(int))original)(0));
	ASSERT_EQ(42, ((int(__cdecl *)(int))original)(10));

	uint32_t target = (uint32_t)cdeclSampleFunctionHook;
	ASSERT_TRUE(HookEngine::HookEngine::instance()->installHook(original, target, &cdeclInfo));

	ASSERT_EQ(42, ((int(__cdecl *)(int))original)(0));
	ASSERT_EQ(52, ((int(__cdecl *)(int))original)(10));
	unloadTarget();
}

// -------------------------------------
typedef int (__stdcall *fnStdcall)(int);
int __fastcall stdcallSampleFunctionHook2(LPVOID sender, fnStdcall original, int x) {

	int result = original(x);
	return result + x;
}

// UNDONE !!!! this tes failed because we have now singleton engine and should reset hook for each test
// Also Loading/unloading(when i finished it) should help 
TEST(IncrimentalLinking, DISABLED_stdcallSimpleInstall) 
{
	HMODULE dll = loadTarget(); 
	ASSERT_NE((HMODULE)0, dll);
	uint32_t original = (uint32_t)GetProcAddress(dll, "stdcallSampleFunction");
	ASSERT_NE(0, original);
	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(0));
	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(10));

	ASSERT_TRUE(HookEngine::HookEngine::instance()->fasthook(dll, "stdcallSampleFunction", stdcallSampleFunctionHook2));

	ASSERT_EQ(42, ((int(__stdcall *)(int))original)(0));
	ASSERT_EQ(52, ((int(__stdcall *)(int))original)(10));
	unloadTarget();
}



