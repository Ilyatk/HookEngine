#pragma once

#include <HookEngine/HookInfo.h>
#include <HookEngine/HookEngine.h>

#include <functional>

namespace HookEngine {
  struct _Empty {};

  struct Stdcall {};
  struct Fastcall {};
  struct Thiscall {};
  struct Cdecl {};

  template <class T>
  struct IsCdecl
  {
    enum { Value = 0 };
  };

  template <>
  struct IsCdecl<Cdecl>
  {
    enum { Value = 1 };
  };
}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

#define CAT_DEFAULT_FAULE(x) x ## = _Empty

#define _classDefinition0
#define _classDefinition1  CAT_DEFAULT_FAULE(class Arg1)
#define _classDefinition2  _classDefinition1, CAT_DEFAULT_FAULE(class Arg2)
#define _classDefinition3  _classDefinition2, CAT_DEFAULT_FAULE(class Arg3)
#define _classDefinition4  _classDefinition3, CAT_DEFAULT_FAULE(class Arg4)
#define _classDefinition5  _classDefinition4, CAT_DEFAULT_FAULE(class Arg5)
#define _classDefinition6  _classDefinition5, CAT_DEFAULT_FAULE(class Arg6)
#define _classDefinition7  _classDefinition6, CAT_DEFAULT_FAULE(class Arg7)
#define _classDefinition8  _classDefinition7, CAT_DEFAULT_FAULE(class Arg8)
#define _classDefinition9  _classDefinition8, CAT_DEFAULT_FAULE(class Arg9)
#define _classDefinition10  _classDefinition9, CAT_DEFAULT_FAULE(class Arg10)
#define _classDefinition11  _classDefinition10, CAT_DEFAULT_FAULE(class Arg11)
#define _classDefinition12  _classDefinition11, CAT_DEFAULT_FAULE(class Arg12)

#define _classDefinitionNoDefault0
#define _classDefinitionNoDefault1  class Arg1
#define _classDefinitionNoDefault2  _classDefinitionNoDefault1, class Arg2
#define _classDefinitionNoDefault3  _classDefinitionNoDefault2, class Arg3
#define _classDefinitionNoDefault4  _classDefinitionNoDefault3, class Arg4
#define _classDefinitionNoDefault5  _classDefinitionNoDefault4, class Arg5
#define _classDefinitionNoDefault6  _classDefinitionNoDefault5, class Arg6
#define _classDefinitionNoDefault7  _classDefinitionNoDefault6, class Arg7
#define _classDefinitionNoDefault8  _classDefinitionNoDefault7, class Arg8
#define _classDefinitionNoDefault9  _classDefinitionNoDefault8, class Arg9
#define _classDefinitionNoDefault10  _classDefinitionNoDefault9, class Arg10
#define _classDefinitionNoDefault11  _classDefinitionNoDefault10, class Arg11
#define _classDefinitionNoDefault12  _classDefinitionNoDefault11, class Arg12

#define _argDefinition0
#define _argDefinition1  Arg1
#define _argDefinition2  _argDefinition1, Arg2
#define _argDefinition3  _argDefinition2, Arg3
#define _argDefinition4  _argDefinition3, Arg4
#define _argDefinition5  _argDefinition4, Arg5
#define _argDefinition6  _argDefinition5, Arg6
#define _argDefinition7  _argDefinition6, Arg7
#define _argDefinition8  _argDefinition7, Arg8
#define _argDefinition9  _argDefinition8, Arg9
#define _argDefinition10  _argDefinition9, Arg10
#define _argDefinition11  _argDefinition10, Arg11
#define _argDefinition12  _argDefinition11, Arg12

#define _argDefinition_thiscall0
#define _argDefinition_thiscall1  
#define _argDefinition_thiscall2  Arg2
#define _argDefinition_thiscall3  _argDefinition_thiscall2, Arg3
#define _argDefinition_thiscall4  _argDefinition_thiscall3, Arg4
#define _argDefinition_thiscall5  _argDefinition_thiscall4, Arg5
#define _argDefinition_thiscall6  _argDefinition_thiscall5, Arg6
#define _argDefinition_thiscall7  _argDefinition_thiscall6, Arg7
#define _argDefinition_thiscall8  _argDefinition_thiscall7, Arg8
#define _argDefinition_thiscall9  _argDefinition_thiscall8, Arg9
#define _argDefinition_thiscall10  _argDefinition_thiscall9, Arg10
#define _argDefinition_thiscall11  _argDefinition_thiscall10, Arg11
#define _argDefinition_thiscall12  _argDefinition_thiscall11, Arg12

#define _argDefinition_fastcall0
#define _argDefinition_fastcall1  
#define _argDefinition_fastcall2
#define _argDefinition_fastcall3  Arg3
#define _argDefinition_fastcall4  _argDefinition_fastcall3, Arg4
#define _argDefinition_fastcall5  _argDefinition_fastcall4, Arg5
#define _argDefinition_fastcall6  _argDefinition_fastcall5, Arg6
#define _argDefinition_fastcall7  _argDefinition_fastcall6, Arg7
#define _argDefinition_fastcall8  _argDefinition_fastcall7, Arg8
#define _argDefinition_fastcall9  _argDefinition_fastcall8, Arg9
#define _argDefinition_fastcall10  _argDefinition_fastcall9, Arg10
#define _argDefinition_fastcall11  _argDefinition_fastcall10, Arg11
#define _argDefinition_fastcall12  _argDefinition_fastcall11, Arg12

#define _argWithValueDefinition0
#define _argWithValueDefinition1  Arg1 a1
#define _argWithValueDefinition2  _argWithValueDefinition1, Arg2 a2
#define _argWithValueDefinition3  _argWithValueDefinition2, Arg3 a3
#define _argWithValueDefinition4  _argWithValueDefinition3, Arg4 a4
#define _argWithValueDefinition5  _argWithValueDefinition4, Arg5 a5
#define _argWithValueDefinition6  _argWithValueDefinition5, Arg6 a6
#define _argWithValueDefinition7  _argWithValueDefinition6, Arg7 a7
#define _argWithValueDefinition8  _argWithValueDefinition7, Arg8 a8
#define _argWithValueDefinition9  _argWithValueDefinition8, Arg9 a9
#define _argWithValueDefinition10  _argWithValueDefinition9, Arg10 a10
#define _argWithValueDefinition11  _argWithValueDefinition10, Arg11 a11
#define _argWithValueDefinition12  _argWithValueDefinition11, Arg12 a12

#define _argWithValueDefinition_thiscall0
#define _argWithValueDefinition_thiscall1  
#define _argWithValueDefinition_thiscall2  Arg2 a2
#define _argWithValueDefinition_thiscall3  _argWithValueDefinition_thiscall2, Arg3 a3
#define _argWithValueDefinition_thiscall4  _argWithValueDefinition_thiscall3, Arg4 a4
#define _argWithValueDefinition_thiscall5  _argWithValueDefinition_thiscall4, Arg5 a5
#define _argWithValueDefinition_thiscall6  _argWithValueDefinition_thiscall5, Arg6 a6
#define _argWithValueDefinition_thiscall7  _argWithValueDefinition_thiscall6, Arg7 a7
#define _argWithValueDefinition_thiscall8  _argWithValueDefinition_thiscall7, Arg8 a8
#define _argWithValueDefinition_thiscall9  _argWithValueDefinition_thiscall8, Arg9 a9
#define _argWithValueDefinition_thiscall10  _argWithValueDefinition_thiscall9, Arg10 a10
#define _argWithValueDefinition_thiscall11  _argWithValueDefinition_thiscall10, Arg11 a11
#define _argWithValueDefinition_thiscall12  _argWithValueDefinition_thiscall11, Arg12 a12

#define _argWithValueDefinition_fastcall0
#define _argWithValueDefinition_fastcall1  
#define _argWithValueDefinition_fastcall2  
#define _argWithValueDefinition_fastcall3  Arg3 a3
#define _argWithValueDefinition_fastcall4  _argWithValueDefinition_fastcall3, Arg4 a4
#define _argWithValueDefinition_fastcall5  _argWithValueDefinition_fastcall4, Arg5 a5
#define _argWithValueDefinition_fastcall6  _argWithValueDefinition_fastcall5, Arg6 a6
#define _argWithValueDefinition_fastcall7  _argWithValueDefinition_fastcall6, Arg7 a7
#define _argWithValueDefinition_fastcall8  _argWithValueDefinition_fastcall7, Arg8 a8
#define _argWithValueDefinition_fastcall9  _argWithValueDefinition_fastcall8, Arg9 a9
#define _argWithValueDefinition_fastcall10  _argWithValueDefinition_fastcall9, Arg10 a10
#define _argWithValueDefinition_fastcall11  _argWithValueDefinition_fastcall10, Arg11 a11
#define _argWithValueDefinition_fastcall12  _argWithValueDefinition_fastcall11, Arg12 a12

#define _valueDefinition0
#define _valueDefinition1 a1
#define _valueDefinition2 _valueDefinition1, a2
#define _valueDefinition3 _valueDefinition2, a3
#define _valueDefinition4 _valueDefinition3, a4
#define _valueDefinition5 _valueDefinition4, a5
#define _valueDefinition6 _valueDefinition5, a6
#define _valueDefinition7 _valueDefinition6, a7
#define _valueDefinition8 _valueDefinition7, a8
#define _valueDefinition9 _valueDefinition8, a9
#define _valueDefinition10 _valueDefinition9, a10
#define _valueDefinition11 _valueDefinition10, a11
#define _valueDefinition12 _valueDefinition11, a12

#define _valueDefinition_thiscall0
#define _valueDefinition_thiscall1 
#define _valueDefinition_thiscall2 a2
#define _valueDefinition_thiscall3 _valueDefinition_thiscall2, a3
#define _valueDefinition_thiscall4 _valueDefinition_thiscall3, a4
#define _valueDefinition_thiscall5 _valueDefinition_thiscall4, a5
#define _valueDefinition_thiscall6 _valueDefinition_thiscall5, a6
#define _valueDefinition_thiscall7 _valueDefinition_thiscall6, a7
#define _valueDefinition_thiscall8 _valueDefinition_thiscall7, a8
#define _valueDefinition_thiscall9 _valueDefinition_thiscall8, a9
#define _valueDefinition_thiscall10 _valueDefinition_thiscall9, a10
#define _valueDefinition_thiscall11 _valueDefinition_thiscall10, a11
#define _valueDefinition_thiscall12 _valueDefinition_thiscall11, a12

#define _valueDefinition_fastcall0
#define _valueDefinition_fastcall1 
#define _valueDefinition_fastcall2 
#define _valueDefinition_fastcall3 a3
#define _valueDefinition_fastcall4 _valueDefinition_fastcall3, a4
#define _valueDefinition_fastcall5 _valueDefinition_fastcall4, a5
#define _valueDefinition_fastcall6 _valueDefinition_fastcall5, a6
#define _valueDefinition_fastcall7 _valueDefinition_fastcall6, a7
#define _valueDefinition_fastcall8 _valueDefinition_fastcall7, a8
#define _valueDefinition_fastcall9 _valueDefinition_fastcall8, a9
#define _valueDefinition_fastcall10 _valueDefinition_fastcall9, a10
#define _valueDefinition_fastcall11 _valueDefinition_fastcall10, a11
#define _valueDefinition_fastcall12 _valueDefinition_fastcall11, a12

#define _tailDefinition_12(x)
#define _tailDefinition_11(x)       x
#define _tailDefinition_10(x)				_tailDefinition_11(x), x
#define _tailDefinition_9(x)				_tailDefinition_10(x), x
#define _tailDefinition_8(x)				_tailDefinition_9(x), x
#define _tailDefinition_7(x)				_tailDefinition_8(x), x
#define _tailDefinition_6(x)				_tailDefinition_7(x), x
#define _tailDefinition_5(x)				_tailDefinition_6(x), x
#define _tailDefinition_4(x)				_tailDefinition_5(x), x
#define _tailDefinition_3(x)				_tailDefinition_4(x), x
#define _tailDefinition_2(x)				_tailDefinition_3(x), x
#define _tailDefinition_1(x)				_tailDefinition_2(x), x
#define _tailDefinition_0(x)				_tailDefinition_1(x), x

#define _HOOK_INCLUDE_ERROR_GUARD_

// It's not enough to change this define for more args
#define MaxSupportedArgsCount 12

#define HookArgsCount 12
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 11
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 10
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 9
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 8
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 7
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 6
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 5
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 4
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 3
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 2
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 1
#include <HookEngine/HookMacro.h>
#undef HookArgsCount

#define HookArgsCount 0
#include <HookEngine/HookMacro.h>
#undef HookArgsCount


