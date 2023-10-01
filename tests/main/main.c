#include "unity_fixture.h"
#include "vibootloadercore.h"

static void runAllTests(void)
{
  RUN_TEST_GROUP(TST_VIBOOTCORE);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, runAllTests);
}