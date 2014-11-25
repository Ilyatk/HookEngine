// SampleDll2.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SampleDll2.h"


// This is an example of an exported variable
SAMPLEDLL2_API int nSampleDll2=0;

// This is an example of an exported function.
SAMPLEDLL2_API int fnSampleDll2(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see SampleDll2.h for the class definition
CSampleDll2::CSampleDll2()
{
	return;
}

int __stdcall fixHookTarget(int a1, int a2, int a3)
{
  return 100 * a1 + 10 * a2 + a3;
}

int __stdcall fixHookTarget2(int a1, int a2, int a3)
{
  return 1000 * a1 + 10 * a2 + a3;
}