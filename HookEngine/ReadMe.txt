TODO

Hook short description.
 // *------------------------------
    // 1. Simple hook
    //     
    // Real
    //  jmp <hook1>
    //  ... garbage
    //  <point1>End of original function
    // 
    // Cave:
    //  - <cave_start>Some operand from original function
    //  - jmp to <point1>
    //
    // Hook:
    //  <hook1>
    //  
    // 2. Fast __fastcall function with edx = original function
    // Real:
    //  jmp <ORIGINAL_FUNCTION_PROLOG_LEN>
    //  ... garbage
    //  <point1>End of original function
    //  
    // Cave:
    //  - <cave_addr>Some operand from original function
    //  - jmp to <point1>
    //  ... garbege
    //  - <ORIGINAL_FUNCTION_PROLOG_LEN> mov edx, <cave_addr>
    //  - jmp <hook1>
    //   
    // Hook:
    //  <hook1>
    //  
    // 3. Fast __fastcall function with edx = original function and third arg = recivers
    // Real:
    //  jmp <ORIGINAL_FUNCTION_PROLOG_LEN>
    //  ... garbage
    //  <point1>End of original function
    //  
    // Cave:
    //  - <cave_addr>Some operand from original function
    //  - jmp to <point1>
    //  ... garbege
    //  - <ORIGINAL_FUNCTION_PROLOG_LEN> mov edx, <cave_addr>
    //  - push <reciever> 68 78 56 34 12
    //  - jmp <hook1>
    //   
    // Hook:
    //  <hook1>
    // -------------------------------

   __fastcall f1(a1 <ecx>, a2<edx>, a3,a4,..)
   
   Stack:
	return
	a3
	a4

   Asm code for prepair hook
	pop ebx
	push edx
	mov edx, fnOriginal
	push receiver
	push ebx

	CPU Disasm
Address   Hex dump          Command                                  Comments
004905B2      5B            POP EBX
004905B3      52            PUSH EDX
004905B4      BA 44332211   MOV EDX,11223344
004905B9      68 44332211   PUSH 11223344
004905BE      53            PUSH EBX


 __fastcall f1_fix(a1 <ecx>, fnOriginal<edx>, receiver, a2, a3,a4,..)


 For some hooks you should use next substitution
 __fastcall f()  => __stdcall f()
 __fastcall f(A a) => __thiscall (A sender)