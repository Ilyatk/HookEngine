HookEngine API Overview
=======================

HookEngine is a professional hooking engine for instrumenting arbitrary Win32 functions, COM objects, and functions 
which symbols are located in program databases (PDBs). It can intercept unmanaged code in 32-bit and 64-bit applications.
It is implemented as a native component, writed with C++. HookEngine created in 2013 and widely used in GameNet application.

HookEngine offers a unique programmer friendly API which resolves the complexities associated with binary
instrumentation so that even software engineers without expertise in the field can use it. Deviare takes care of code
injection and parameter marshalling. Intercepting applications is a complex task. We test multiple application environments
to ensure that the end user has a trouble-free experience. HookEngine also has a focus on maximum ease of use as soon as possible.

It can be used in several like: tracing and debugging, sandboxing and browser security, malware analysis, video 
conference recording, and gaming.

Why HookEngine?
===============
* Used in real life application with vast geography and many millions of users.
* Well tested
* Supports stdcall, template, thiscall hooks out of box.
* Simple - less then 5 minuted before you start.

Requirements
============

    Microsoft Visual Studio 2010

Simple Usage
============
Let`s try to make hook for MessageBoxA function from

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

        HookEngine::instance()
            ->fasthook(original, (uint32_t)MessageBoxAHook);

        MessageBoxA(0, "Not hooked yet", "Warning", MB_OK);
    }

And it can be even more simple:

    #include <HookEngine/HookEngine.h>
    #include <Windows.h>

    void main()
    {
        auto messageBoxAHook = HookEngine::createHook<
            HookEngine::Stdcall, // target function calling convention
            int,                 // return type
            HWND hWnd,           //first argument type
            LPCSTR,              //second argument type
            LPCSTR,              //third argument type
            UINT                 //fourth argument type
        >("User32.dll", "MessageBoxA");

      // Set hook function.
        messageBoxAHook->assignHook([messageBoxAHook](HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> int {
            messageBoxAHook->original(0, "Hello from my first Hook!", "Warning", MB_OK);  //Call our payload
            return messageBoxAHook->original(hWnd, lpText, lpCaption, uType);             //and original
        });
    }

Advanced Usage
==============
Look at `Example` folder for more examples of usage. Fill free to add Feature Request of question.

Credits
==============
Originally HookEngine was written as a part of [GameNet project](http://gamenet.ru) by Ilya Tkachenko (ilyamsu at gmail.com)
the lead developer of GameNet application.

License
==============
Released under the [MIT](LICENSE) license.