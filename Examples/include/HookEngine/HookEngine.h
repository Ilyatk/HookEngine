#pragma once

#include <HookEngine/Hook.h>
#include <HookEngine/HookInfo.h>
#include <HookEngine/CodeCaveFinder.h>
#include <HookEngine/System/Singleton.h>
#include <HookEngine/System/CriticalSection.h>

#include <HookEngine/LoadLibraryHook.h>

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#include <Windows.h>

extern "C" int __cdecl mlde32(void *codeptr);

namespace HookEngine {

  /*!
  \fn template<typename T> uint32_t ClassFuntionPointerToUint32(T p)
  \brief Cast class member function address to uint32_t.
  \author Ilya.Tkachenko
  \date 12.04.2013
  \param p The.
  \return .
  */
  template<typename T> uint32_t ClassFuntionPointerToUint32(T p) {
    uint32_t tmp;
    __asm {
      mov eax, p;
      mov tmp, eax;
    }
    return tmp;
  }

  typedef std::unordered_map<std::wstring, std::vector<HookInfo *> > HookByModuleMap;

  class HookEngine : public System::Singleton<HookEngine>
  {
  public:

    /*!
    \fn void :::init();
    \brief Initializes this object. Should be called before using any persistent hooks.
    \author Ilya.Tkachenko
    \date 27.05.2013
    */
    void init();

    bool fasthook(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo = NULL);

    bool fasthook(LPCSTR dllName, LPCSTR functionName, uint32_t hookAddr, HookInfo** hookInfo = NULL);

    bool fasthook(HMODULE dll, LPCSTR functionName, uint32_t hookAddr, HookInfo** hookInfo = NULL);

    // просто обертка чтобы не каститить перед вызовом функцию в uint32_t
    template <typename T> bool fasthook(HMODULE dll, LPCSTR functionName, T hookFunc, HookInfo** hookInfo = NULL) 
    {
      return this->fasthook(dll, functionName, (uint32_t)hookFunc, hookInfo);
    }

    // хук общего назначения - просто ставит трамплин на указанный адресс не меня ничего
    bool installHook(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo = NULL);

    // Устанавливает хук на __stdcall функцию. При этом вызывается instance->hookAddr 
    bool installStdHook(uint32_t realAddr, uint32_t intance, uint32_t hookAddr, HookInfo** hookInfo);

    // WARNING be carefully with hookInfo - you shouldn't delete it.
    bool fasthookForTemplate(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo);
    bool fasthookForTemplateCdecl(uint32_t realAddr, uint32_t hookAddr, HookInfo** hookInfo);

    // WARNING Works only for template hook
    void registerForRestore(HookInfo *hookInfo);

    void onModuleLoaded(std::function<void()> callback);
    void onModuleUnloaded(std::function<void()> callback);

    void setLogger(std::function<void(const std::string&)> logger);
    void log(const std::string& msg);

  protected:
    friend class System::Singleton<HookEngine>;
    friend class LoadLibraryHook;

    HookEngine();
    ~HookEngine();

  private:
    HookByModuleMap _activeHook;

    CodeCaveFinder _codeCaveFinder;
    std::map<uint32_t, HookInfo *> _hookes;

    System::CriticalSection _criticalSection;
    LoadLibraryHook _loadLibraryHooks;

    std::function<void()> _onModuleLoaded;
    std::function<void()> _onModuleUnloaded;

    std::function<void(const std::string& msg)> _externalLogger;

    std::map<HMODULE, std::string> _modulesNameMap;

    void moduleLoaded(HMODULE module);
    void moduleUnloaded(HMODULE module);

    void fixTrampoline(int64_t baseAddress, HookInfo *info);
    bool isHookBroken(uint32_t realAddr, uint32_t targetAddr);

    std::wstring getModuleFullPath(HMODULE module);
    bool getHook(uint32_t realAddr, HookInfo** hook);
  };
}
