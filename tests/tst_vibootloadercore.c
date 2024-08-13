#include "unity.h"
#include "unity_fixture.h"
#include "vibootloadercore.h"
#include "stdio.h"

TEST_GROUP(TST_VIBOOTCORE);

TEST_GROUP_RUNNER(TST_VIBOOTCORE) {
  RUN_TEST_CASE(TST_VIBOOTCORE, VIBCORE_Runtime);
}

TEST_SETUP(TST_VIBOOTCORE) {
}

TEST_TEAR_DOWN(TST_VIBOOTCORE) {
}

TEST(TST_VIBOOTCORE, VIBCORE_Runtime)
{
  printf("Test: VIBCORE_Runtime");
  TEST_ASSERT_EQUAL(0, VIBCORE_Runtime());
}