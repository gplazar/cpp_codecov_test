#include "gtest/gtest.h"
#include "example.h"

TEST(example, divide)
{
    double res;
    res = divide_numbers(2.0, 1.0);
    ASSERT_NEAR(res, 2.0, 1.0e-11);
}
