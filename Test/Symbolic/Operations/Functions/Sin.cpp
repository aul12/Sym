#include "Symbolic/Operations/Functions/Sin.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Sin, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Sin<Const>>::val));
}

TEST(Sin, Resolve) {
    sym::Constant<double> a{17};
    sym::Sin<decltype(a)> sin{a};
    EXPECT_DOUBLE_EQ(sin.resolve(), std::sin(17));
}

TEST(Sin, Gradient) {
    sym::Variable<double, 'a'> x{0};
    sym::Sin<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(), 1);
}
