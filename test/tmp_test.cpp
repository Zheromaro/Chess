#include <gtest/gtest.h>


TEST(Something, CheckValues)
{
  ASSERT_EQ(3, 3);
  EXPECT_NE(2, 3);
  EXPECT_TRUE(true);
}

TEST(Something, CheckValuesAgain)
{
  ASSERT_EQ(3, 3);
  EXPECT_NE(2, 3);
  EXPECT_TRUE(true);
}
