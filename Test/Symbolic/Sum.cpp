#include "Symbolic/Sum.hpp"

#include <gtest/gtest.h>

TEST(Sum, Incr) {
    auto f = [](auto i) { return sym::RuntimeConstant<double>(i); };
    auto sum = sym::make_sum<0, 100>(f);
    EXPECT_DOUBLE_EQ(sum.resolve(), (100 * 99) / 2);
}
