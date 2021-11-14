#include "Symbolic/Operations/Functions/Tan.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Tan, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Tan<Const>>::val));
}

TEST(Tan, Resolve) {
    sym::RuntimeConstant<double> a{17};
    sym::Tan<decltype(a)> tan{a};
    EXPECT_DOUBLE_EQ(tan.resolve(), std::tan(17));
}

TEST(Tan, Gradient) {
    sym::Variable<'x'> x;
    sym::Tan<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 1);
}
