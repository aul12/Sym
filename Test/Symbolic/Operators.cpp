#include "Symbolic/Operators.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Operators, OperatorPlus) {
    sym::Constant<int> c{42};
    auto res = c + c;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Add<decltype(c), decltype(c)>>) );
    EXPECT_EQ(res.resolve(), 42 + 42);
}

TEST(Operators, OperatorTimes) {
    sym::Constant<int> c{42};
    auto res = c * c;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Mul<decltype(c), decltype(c)>>) );
    EXPECT_EQ(res.resolve(), 42 * 42);
}

TEST(Operators, OperatorDiv) {
    sym::Constant<double> c{42};
    sym::Constant<double> d{17};
    auto res = c / d;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Div<decltype(c), decltype(d)>>) );
    EXPECT_DOUBLE_EQ(res.resolve(), 42.0 / 17);
}

TEST(Operators, OperatorSub) {
    sym::Constant<int> c{42};
    sym::Constant<int> d{17};
    auto res = c - d;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Sub<decltype(c), decltype(d)>>) );
    EXPECT_DOUBLE_EQ(res.resolve(), 42 - 17);
}
