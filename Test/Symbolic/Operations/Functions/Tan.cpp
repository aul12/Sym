#include "Symbolic/Operations/Functions/Tan.hpp"

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
    sym::Variable<double, 'x'> x;
    sym::Tan<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 1);
}
