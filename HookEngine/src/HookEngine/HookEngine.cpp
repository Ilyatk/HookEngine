#include <HookEngine/HookEngine.h>
#include <HookEngine/System/Singleton.h>

namespace HookEngine {

  class HookEngineInstnce : public System::Singleton<HookEngineInstnce>
  {
  public:
    HookEngine* engine;

  protected:
    friend class System::Singleton<HookEngineInstnce>;

    HookEngineInstnce() 
      : engine(new HookEngine)
    {
    }

    ~HookEngineInstnce()
    {
      delete engine;
    }
  };

  HookEngine* instance()
  {
    return HookEngineInstnce::instance()->engine;
  }

}