#include "Symbolic/Operations/Functions/ArcTan.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(ArcTan, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::ArcTan<Const>>::val));
}

TEST(ArcTan, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcTan<decltype(a)> atan{a};
    EXPECT_DOUBLE_EQ(atan.resolve(), std::atan(0.5));
}

TEST(ArcTan, Gradient) {
    sym::Variable<'x'> x;
    sym::ArcTan<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 1);
}
