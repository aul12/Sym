#include "Symbolic/Operations/Functions/ArcCos.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(ArcCos, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::ArcCos<Const>>::val));
}

TEST(ArcCos, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcCos<decltype(a)> acos{a};
    EXPECT_DOUBLE_EQ(acos.resolve(), std::acos(0.5));
}

TEST(ArcCos, ToString) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcCos<decltype(a)> acos{a};
    EXPECT_NO_THROW(sym::toString(acos));
}

TEST(ArcCos, Gradient) {
    sym::Variable<'x'> x;
    sym::ArcCos<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), -1);
}
