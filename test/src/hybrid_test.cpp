#include "hybridstructures/hybridDS.hpp"

#include <gtest/gtest.h>

TEST(HybridSubtractTest, CheckValues)
{
  ASSERT_EQ(hybrid::subtract(3, 2), 1);
  EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
