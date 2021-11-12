#include "Symbolic/Functions/Tan.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Tan, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Tan<Const>>::val));
}

TEST(Tan, Resolve) {
    sym::Constant<double> a{17};
    sym::Tan<decltype(a)> tan{a};
    EXPECT_DOUBLE_EQ(tan.resolve(), std::tan(17));
}

TEST(Tan, Gradient) {
    sym::Variable<double> x{0};
    sym::Tan<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(), 1);
}

TEST(Tan, IsConstantC) {
    using C = sym::Constant<int>;
    EXPECT_TRUE((sym::Tan<C>::isConstant()));
}

TEST(Tan, IsConstantV) {
    using V = sym::Variable<int>;
    EXPECT_FALSE((sym::Tan<V>::isConstant()));
}
