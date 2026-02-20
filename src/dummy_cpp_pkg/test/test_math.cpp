#include "gtest/gtest.h"
#include "dummy_cpp_pkg/math_utils.hpp"

TEST(MathUtilsTest, AddPositiveNumbers)
{
  EXPECT_DOUBLE_EQ(dummy_cpp_pkg::add(2.0, 3.0), 5.0);
}

TEST(MathUtilsTest, SubtractPositiveNumbers)
{
  EXPECT_EQ(dummy_cpp_pkg::sub(5.0, 3.0), 2.0);
}
TEST(MathUtilsTest, MultiplyPositiveNumbers)
{
  EXPECT_EQ(dummy_cpp_pkg::multiply(2.0, 3.0), 6.0);
}