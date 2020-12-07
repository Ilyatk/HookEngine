
namespace HookEngine {

#ifdef WITHOUT_TAIL
  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class Hook;

  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class StaticHookHelper;
#endif

#if HookArgsCount == 0
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType(*fnOriginal)();
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress();
    }

    void assignHook(std::function<ReturnType(argDefinition)> hook)
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(*fnOriginal)();

    static ReturnType hook(
      uintptr_t a1,
      uintptr_t a2,
      uintptr_t a3,
      uintptr_t a4,
      uintptr_t caller,
      HookInfo* hookInfo)
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())();

      return ((Hook<Win64, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#endif

#if HookArgsCount == 1
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType(*fnOriginal)(Arg1 a1);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1);
    }

    void assignHook(std::function<ReturnType(argDefinition)> hook)
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(*fnOriginal)(Arg1 a1);

    static ReturnType hook(
      Arg1 a1,
      uintptr_t a2,
      uintptr_t a3,
      uintptr_t a4,
      uintptr_t caller,
      HookInfo* hookInfo)
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1);

      return ((Hook<Win64, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#endif

#if HookArgsCount == 2
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1, a2);
    }

    void assignHook(std::function<ReturnType(argDefinition)> hook)
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2);

    static ReturnType hook(
      Arg1 a1,
      Arg2 a2,
      uintptr_t a3,
      uintptr_t a4,
      uintptr_t caller,
      HookInfo* hookInfo)
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1, a2);

      return ((Hook<Win64, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#endif

#if HookArgsCount == 3
  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2, Arg3 a3);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1, a2, a3);
    }

    void assignHook(std::function<ReturnType(argDefinition)> hook)
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2, Arg3 a3);

    static ReturnType hook(
      Arg1 a1,
      Arg2 a2,
      Arg3 a3,
      uintptr_t a4,
      uintptr_t caller,
      HookInfo* hookInfo)
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1, a2, a3);

      return ((Hook<Win64, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#endif

#if HookArgsCount > 3

  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (this->_hookInfo)
        this->_hookInfo->setReceiver(0);
    }

    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4 COMMA_win64call argDefinition_win64call);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1, a2, a3, a4 COMMA_win64call valueDefinition_win64call);
    }

    void assignHook(std::function<ReturnType(argDefinition)> hook)
    {
      this->_hook = hook;
      this->_hookInfo->setReceiver(this->_hook ? this : 0);
    }

    void setHookInfo(HookInfo *info)
    {
      this->_hookInfo = info;
      this->_originalAddress = (fnOriginal)info->afterPtr();
    }

  private:
    HookInfo *_hookInfo;
    fnOriginal _originalAddress;
    hookType _hook;
  };

  template <class ReturnType COMMA classDefinitionNoDefault>
  class StaticHookHelper<Win64, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(*fnOriginal)(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4 COMMA_win64call argDefinition_win64call);

    static ReturnType hook(
      Arg1 a1, 
      Arg2 a2, 
      Arg3 a3, 
      Arg4 a4,
      uintptr_t caller,
      HookInfo* hookInfo COMMA_win64call argWithValueDefinition_win64call)
    {
      if (!hookInfo->receiver())
        return ((fnOriginal)hookInfo->afterPtr())(a1, a2, a3, a4 COMMA_win64call valueDefinition_win64call);

      return ((Hook<Win64, ReturnType COMMA argDefinition> *)hookInfo->receiver())->hookCalled(valueDefinition);
    }

  private:
    StaticHookHelper(){};
    ~StaticHookHelper(){};
  };

#endif

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(const wchar_t* dllName, const char* name)
  {
    HMODULE dll = GetModuleHandleW(dllName);
    if (!dll)
      return NULL;

    uintptr_t target = (uintptr_t)GetProcAddress(dll, name);
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

    uintptr_t target = (uintptr_t)GetProcAddress(dll, name);
    if (!target)
      return NULL;

    return createHook<CallingConvention, ReturnType COMMA argDefinition>(target);
  }

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(HMODULE dll, const char* name)
  {
    uintptr_t target = (uintptr_t)GetProcAddress(dll, name);
    if (!target)
      return NULL;

    return createHook<CallingConvention, ReturnType COMMA argDefinition>(target);
  }

  template<class CallingConvention, class ReturnType COMMA classDefinitionNoDefault>
  Hook<CallingConvention, ReturnType COMMA argDefinition>* createHook(uintptr_t targetAddress)
  {
    HookInfo *info = NULL;

    if (!::HookEngine::instance()->installHookForTemplate(
      targetAddress,
      (uintptr_t)StaticHookHelper<CallingConvention, ReturnType COMMA argDefinition>::hook,
      &info)) {
      // If we can't set hook or hook is not empty then return NULL
      if (info == NULL) {
        return NULL;
      }

      if (info->receiver() != 0) {
        return NULL;
      }
    }

    Hook<CallingConvention, ReturnType COMMA argDefinition> *result =
      new Hook<CallingConvention, ReturnType COMMA argDefinition>();

    result->setHookInfo(info);

    return result;
  }
}
