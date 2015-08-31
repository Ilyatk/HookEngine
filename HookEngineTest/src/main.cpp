#include <HookEngine/HookEngine.h>
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int r = RUN_ALL_TESTS();

  return r;
}