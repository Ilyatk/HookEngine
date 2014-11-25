#include <gtest/gtest.h>
#include <HookEngine/HookEngine.h>

#include <iostream>

int main(int argc, char **argv) 
{
  HookEngine::HookEngine::instance()->init();
  ::testing::InitGoogleTest(&argc, argv);
  int r = RUN_ALL_TESTS();

  std::cout << std::endl << "HookProfile: " << std::endl
    << HookEngine::Profiler::instance()->result() << std::endl;
  return r;
}
