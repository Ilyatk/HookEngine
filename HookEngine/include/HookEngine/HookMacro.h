#ifndef _HOOK_INCLUDE_ERROR_GUARD_
  #error Include Hook.h insted of HookMacro.h
#endif

#ifndef HookArgsCount
  #error HookArgsCount undefiend
#endif

#undef COMMA
#if HookArgsCount == 0
  #define COMMA
#else 
  #define COMMA ,
#endif 

#undef COMMA_thiscall
#if HookArgsCount > 1
  #define COMMA_thiscall ,
#else
  #define COMMA_thiscall
#endif

#undef COMMA_fastcall
#if HookArgsCount > 2
  #define COMMA_fastcall ,
#else
  #define COMMA_fastcall
#endif

#undef COMMA_win64call
#if HookArgsCount > 4
#define COMMA_win64call ,
#else
#define COMMA_win64call
#endif

// class Arg1 = _Empty, class Arg2 = _Empty, ...
#define classDefinition PPCAT(_classDefinition, HookArgsCount)

#define classDefinitionNoDefault PPCAT(_classDefinitionNoDefault, HookArgsCount)

// Arg1, Arg2, ...
#define argDefinition PPCAT(_argDefinition, HookArgsCount)

// Arg1 a1, Arg2 a2, ...
#define argWithValueDefinition PPCAT(_argWithValueDefinition, HookArgsCount)

// a1, a2, ...
#define valueDefinition PPCAT(_valueDefinition, HookArgsCount)

#define TAIL(x) PPCAT(_tailDefinition_, HookArgsCount)(x)
#define emptyTail TAIL(_Empty)

// For this call some new defines
// Arg2, ...
#define argDefinition_thiscall PPCAT(_argDefinition_thiscall, HookArgsCount)
#define valueDefinition_thiscall PPCAT(_valueDefinition_thiscall, HookArgsCount)
#define argWithValueDefinition_thiscall PPCAT(_argWithValueDefinition_thiscall, HookArgsCount)

#define argDefinition_fastcall PPCAT(_argDefinition_fastcall, HookArgsCount)
#define valueDefinition_fastcall PPCAT(_valueDefinition_fastcall, HookArgsCount)
#define argWithValueDefinition_fastcall PPCAT(_argWithValueDefinition_fastcall, HookArgsCount)


#define argDefinition_win64call PPCAT(_argDefinition_win64call, HookArgsCount)
#define valueDefinition_win64call PPCAT(_valueDefinition_win64call, HookArgsCount)
#define argWithValueDefinition_win64call PPCAT(_argWithValueDefinition_win64call, HookArgsCount)

#if HookArgsCount == MaxSupportedArgsCount
  #undef WITHOUT_TAIL
  #define WITHOUT_TAIL 1
#else
  #undef WITHOUT_TAIL
#endif

#undef TAIL_COMMA
#if WITHOUT_TAIL
  #define TAIL_COMMA
#else 
  #define TAIL_COMMA ,
#endif 

// If something wrong uncomment next
//#pragma message("---------------------------------------------------------------------------------------------")
//#pragma message("classDefinition=" TOSTRING(PPCAT(:, classDefinition)))
//#pragma message("classDefinitionNoDefault=" TOSTRING(PPCAT(:, classDefinitionNoDefault)))
//#pragma message("argDefinition=" TOSTRING(PPCAT(:, argDefinition)))
//#pragma message("argWithValueDefinition=" TOSTRING(PPCAT(:, argWithValueDefinition)))
//#pragma message("valueDefinition=" TOSTRING(PPCAT(:, valueDefinition)))
//#pragma message("emptyTail=" TOSTRING(PPCAT(:, emptyTail)))
//#pragma message("COMMA=" TOSTRING(PPCAT(:, COMMA)))
//#pragma message("TAIL_COMMA=" TOSTRING(PPCAT(:, TAIL_COMMA)))
//#pragma message("classDefinitionNoDefault=" TOSTRING(PPCAT(:, classDefinitionNoDefault)))

#ifdef _WIN64
#include <HookEngine/private/x64/HookMacro64.h>
#else
#include <HookEngine/private/x86/HookMacro86.h>
#endif