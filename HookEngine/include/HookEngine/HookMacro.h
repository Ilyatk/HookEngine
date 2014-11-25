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

namespace HookEngine {

#ifdef WITHOUT_TAIL
  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class Hook;

  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class StaticHookHelper;
#endif

  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Stdcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail> : public ProfileItem
  {
  public:
    Hook() : _originalAddress(0) , _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType (__stdcall *fnOriginal)(argDefinition);
    typedef std::function<ReturnType (argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerWatcher watcher(this);
#endif
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerOriginalWatcher watcher(this);
#endif
      return this->_originalAddress(valueDefinition);
    }

    void assignHook(std::function<ReturnType (argDefinition)> hook) 
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

    void makePersistent() 
    {
      HookEngine::instance()->registerForRestore(this->_hookInfo);
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Stdcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType (__stdcall *fnOriginal)(argDefinition);

    static ReturnType __fastcall hook(
      uint32_t ecx,
      uint32_t edx,
      HookInfo* hookInfo COMMA argWithValueDefinition) 
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(valueDefinition);

      return ((Hook<Stdcall, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#if HookArgsCount > 0
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Thiscall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail> : public ProfileItem
  {
  public:
    Hook() : _originalAddress(0) , _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType (__fastcall *fnOriginal)(Arg1 sender, int hz COMMA_thiscall argDefinition_thiscall);
    typedef std::function<ReturnType (argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerWatcher watcher(this);
#endif
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerOriginalWatcher watcher(this);
#endif
      return this->_originalAddress(a1, 0 COMMA_thiscall valueDefinition_thiscall);
    }

    void assignHook(std::function<ReturnType (argDefinition)> hook) 
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

    void makePersistent() 
    {
      HookEngine::instance()->registerForRestore(this->_hookInfo);
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Thiscall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType (__fastcall *fnOriginal)(Arg1 a1, int hz COMMA_thiscall argDefinition_thiscall);

    static ReturnType __fastcall hook(
      Arg1 a1,
      fnOriginal original, 
      HookInfo* hookInfo COMMA_thiscall argWithValueDefinition_thiscall) 
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1, 0 COMMA_thiscall valueDefinition_thiscall);

      return ((Hook<Thiscall, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };
#endif

#if HookArgsCount > 1
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail> : public ProfileItem
  {
  public:
    Hook() : _originalAddress(0) , _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType (__fastcall *fnOriginal)(Arg1 a1, Arg2 a2 COMMA_fastcall argDefinition_fastcall);
    typedef std::function<ReturnType (argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerWatcher watcher(this);
#endif
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerOriginalWatcher watcher(this);
#endif
      return this->_originalAddress(a1, a2 COMMA_fastcall valueDefinition_fastcall);
    }

    void assignHook(std::function<ReturnType (argDefinition)> hook) 
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

    void makePersistent() 
    {
      HookEngine::instance()->registerForRestore(this->_hookInfo);
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType (__fastcall *fnOriginal)(Arg1 a1, Arg2 a2 COMMA_fastcall argDefinition_fastcall);

    static ReturnType __fastcall hook(
      Arg1 a1,
      Arg2 a2,
      HookInfo* hookInfo COMMA_fastcall argWithValueDefinition_fastcall) 
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1, a2 COMMA_fastcall valueDefinition_fastcall);

      return ((Hook<Fastcall, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };
#endif

#if HookArgsCount == 1
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail> : public ProfileItem
  {
  public:
    Hook() : _originalAddress(0) , _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType (__fastcall *fnOriginal)(Arg1 a1);
    typedef std::function<ReturnType (argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerWatcher watcher(this);
#endif
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition) 
    {
      ProfilerOriginalWatcher watcher(this);
      return this->_originalAddress(a1);
    }

    void assignHook(std::function<ReturnType (argDefinition)> hook) 
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

    void makePersistent() 
    {
      HookEngine::instance()->registerForRestore(this->_hookInfo);
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType (__fastcall *fnOriginal)(Arg1 a1);

    static ReturnType __fastcall hook(Arg1 a1, uint32_t hz, HookInfo* hookInfo) 
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1);

      return ((Hook<Stdcall, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };
#endif

  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Cdecl, ReturnType COMMA argDefinition TAIL_COMMA emptyTail> : public ProfileItem
  {
  public:
    Hook() : _originalAddress(0) , _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType (__cdecl *fnOriginal)(argDefinition);
    typedef std::function<ReturnType (argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerWatcher watcher(this);
#endif
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition) 
    {
#ifndef DISABLE_PROFILER
      ProfilerOriginalWatcher watcher(this);
#endif
      return this->_originalAddress(valueDefinition);
    }

    void assignHook(std::function<ReturnType (argDefinition)> hook) 
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

    void makePersistent() 
    {
      HookEngine::instance()->registerForRestore(this->_hookInfo);
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Cdecl, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType (__cdecl *fnOriginal)(argDefinition);

    static ReturnType __cdecl hook(HookInfo* hookInfo, uint32_t hz COMMA argWithValueDefinition) 
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(valueDefinition);

      return ((Hook<Cdecl, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(const wchar_t* dllName, const char* name)
  {
    HMODULE dll = GetModuleHandleW(dllName);
    if (!dll)
      return NULL;

    uint32_t target = (uint32_t)GetProcAddress(dll, name);
    if (!target)
      return NULL;

    return createHook<CallingConvention, ReturnType COMMA argDefinition>(target);
  }

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(const char* dllName, const char* name)
  {
    HMODULE dll = GetModuleHandleA(dllName);
    if (!dll)
      return NULL;

    uint32_t target = (uint32_t)GetProcAddress(dll, name);
    if (!target)
      return NULL;

    return createHook<CallingConvention, ReturnType COMMA argDefinition>(target);
  }

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(HMODULE dll, const char* name)
  {
    uint32_t target = (uint32_t)GetProcAddress(dll, name);
    if (!target)
      return NULL;

    return createHook<CallingConvention, ReturnType COMMA argDefinition>(target);
  }

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(uint32_t targetAddress) 
  {
    HookInfo *info = NULL;
    if (IsCdecl<CallingConvention>::Value) {
      if (!HookEngine::instance()->fasthookForTemplateCdecl(
        targetAddress,
        (uint32_t)StaticHookHelper<CallingConvention, ReturnType COMMA argDefinition>::hook, 
        &info)) {
          // If we can't set hook or hook is not empty then return NULL
          if (info == NULL) {
            HookEngine::instance()->log("Failed to set hook");
            return NULL;
          }

          if (info->receiver() != 0) {
            HookEngine::instance()->log("Function has already been hooked");
            return NULL;
          }
      }
    } else {
      if (!HookEngine::instance()->fasthookForTemplate(
        targetAddress,
        (uint32_t)StaticHookHelper<CallingConvention, ReturnType COMMA argDefinition>::hook, 
        &info)) {
          // If we can't set hook or hook is not empty then return NULL
          if (info == NULL) {
            HookEngine::instance()->log("Failed to set hook");
            return NULL;
          }

          if (info->receiver() != 0) {
            HookEngine::instance()->log("Function has already been hooked");
            return NULL;
          }
      }
    }

    Hook<CallingConvention, ReturnType COMMA argDefinition> *result = 
      new Hook<CallingConvention, ReturnType COMMA argDefinition>();

    result->setHookInfo(info);
    result->setAddress(targetAddress);
    Profiler::instance()->registerItem(result);

    return result;
  }

}