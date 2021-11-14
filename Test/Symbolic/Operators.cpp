#include "Symbolic/Operators.hpp"

#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Operators, OperatorPlus) {
    sym::RuntimeConstant<int> c{42};
    auto res = c + c;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Add<decltype(c), decltype(c)>>) );
    EXPECT_EQ(res.resolve(), 42 + 42);
}

TEST(Operators, OperatorTimes) {
    sym::RuntimeConstant<int> c{42};
    auto res = c * c;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Mul<decltype(c), decltype(c)>>) );
    EXPECT_EQ(res.resolve(), 42 * 42);
}

TEST(Operators, OperatorDiv) {
    sym::RuntimeConstant<double> c{42};
    sym::RuntimeConstant<double> d{17};
    auto res = c / d;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Div<decltype(c), decltype(d)>>) );
    EXPECT_DOUBLE_EQ(res.resolve(), 42.0 / 17);
}

TEST(Operators, OperatorSub) {
    sym::RuntimeConstant<int> c{42};
    sym::RuntimeConstant<int> d{17};
    auto res = c - d;
    EXPECT_TRUE((std::same_as<decltype(res), sym::Sub<decltype(c), decltype(d)>>) );
    EXPECT_DOUBLE_EQ(res.resolve(), 42 - 17);
}
