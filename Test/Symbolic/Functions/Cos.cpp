#include "Symbolic/Functions/Cos.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Cos, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Cos<Const>>::val));
}

TEST(Cos, Resolve) {
    sym::Constant<double> a{17};
    sym::Cos<decltype(a)> cos{a};
    EXPECT_DOUBLE_EQ(cos.resolve(), std::cos(17));
}

TEST(Cos, Gradient) {
    sym::Variable<double> x{0};
    sym::Cos<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(), 0);
}

TEST(Cos, IsConstantC) {
    using C = sym::Constant<int>;
    EXPECT_TRUE((sym::Cos<C>::isConstant()));
}

TEST(Cos, IsConstantV) {
    using V = sym::Variable<int>;
    EXPECT_FALSE((sym::Cos<V>::isConstant()));
}
