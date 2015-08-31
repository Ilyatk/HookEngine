
namespace HookEngine {

#ifdef WITHOUT_TAIL
  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class Hook;

  template <class CallingConvention, class ReturnType COMMA classDefinition>
  class StaticHookHelper;
#endif

  template <class ReturnType COMMA classDefinitionNoDefault>
  class Hook<Stdcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType(__stdcall *fnOriginal)(argDefinition);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(valueDefinition);
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
  class StaticHookHelper<Stdcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(__stdcall *fnOriginal)(argDefinition);

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
  class Hook<Thiscall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType(__fastcall *fnOriginal)(Arg1 sender, int hz COMMA_thiscall argDefinition_thiscall);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1, 0 COMMA_thiscall valueDefinition_thiscall);
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
  class StaticHookHelper<Thiscall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(__fastcall *fnOriginal)(Arg1 a1, int hz COMMA_thiscall argDefinition_thiscall);

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
  class Hook<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
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

    typedef ReturnType(__fastcall *fnOriginal)(Arg1 a1, Arg2 a2 COMMA_fastcall argDefinition_fastcall);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(a1, a2 COMMA_fastcall valueDefinition_fastcall);
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
  class StaticHookHelper<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(__fastcall *fnOriginal)(Arg1 a1, Arg2 a2 COMMA_fastcall argDefinition_fastcall);

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
  class Hook<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
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

    typedef ReturnType(__fastcall *fnOriginal)(Arg1 a1);
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
  class StaticHookHelper<Fastcall, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(__fastcall *fnOriginal)(Arg1 a1);

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
  class Hook<Cdecl, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    Hook() : _originalAddress(0), _hookInfo(0)
    {
    }

    ~Hook()
    {
      if (_hookInfo)
        _hookInfo->setReceiver(0);
    }

    typedef ReturnType(__cdecl *fnOriginal)(argDefinition);
    typedef std::function<ReturnType(argDefinition)> hookType;

    ReturnType hookCalled(argWithValueDefinition)
    {
      return this->_hook(valueDefinition);
    }

    ReturnType original(argWithValueDefinition)
    {
      return this->_originalAddress(valueDefinition);
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
  class StaticHookHelper<Cdecl, ReturnType COMMA argDefinition TAIL_COMMA emptyTail>
  {
  public:
    typedef ReturnType(__cdecl *fnOriginal)(argDefinition);

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
      if (!instance()->fasthookForTemplateCdecl(
        targetAddress,
        (uint32_t)StaticHookHelper<CallingConvention, ReturnType COMMA argDefinition>::hook,
        &info)) {
        // If we can't set hook or hook is not empty then return NULL
        if (info == NULL) {
          return NULL;
        }

        if (info->receiver() != 0) {
          return NULL;
        }
      }
    }
    else {
      if (!instance()->fasthookForTemplate(
        targetAddress,
        (uint32_t)StaticHookHelper<CallingConvention, ReturnType COMMA argDefinition>::hook,
        &info)) {
        // If we can't set hook or hook is not empty then return NULL
        if (info == NULL) {
          return NULL;
        }

        if (info->receiver() != 0) {
          return NULL;
        }
      }
    }

    Hook<CallingConvention, ReturnType COMMA argDefinition> *result =
      new Hook<CallingConvention, ReturnType COMMA argDefinition>();

    result->setHookInfo(info);

    return result;
  }
}
