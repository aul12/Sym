#include "Symbolic/Operations/Functions/ArcSin.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(ArcSin, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::ArcSin<Const>>::val));
}

TEST(ArcSin, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcSin<decltype(a)> asin{a};
    EXPECT_DOUBLE_EQ(asin.resolve(), std::asin(0.5));
}

TEST(ArcSin, ToString) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcSin<decltype(a)> asin{a};
    EXPECT_NO_THROW(sym::toString(asin));
}

TEST(ArcSin, Gradient) {
    sym::Variable<'x'> x;
    sym::ArcSin<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 1);
}
