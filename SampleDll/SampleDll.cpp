// SampleDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SampleDll.h"


// This is an example of an exported variable
SAMPLEDLL_API int nSampleDll=0;

// This is an example of an exported function.
SAMPLEDLL_API int __stdcall stdcallSampleFunction(int a)
{
	return 42;
}


SAMPLEDLL_API int __cdecl cdeclSampleFunction(int a)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see SampleDll.h for the class definition
CSampleDll::CSampleDll()
{
	return;
}

int CSampleDll::test()
{
  return fnSampleDll2();
}
