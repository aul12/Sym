#include "Symbolic/Operations/Functions/Exp.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Exp, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Exp<Const>>::val));
}

TEST(Exp, Resolve) {
    sym::RuntimeConstant<double> a{17};
    sym::Exp<decltype(a)> exp{a};
    EXPECT_DOUBLE_EQ(exp.resolve(), std::exp(17));
}

TEST(Exp, ToString) {
    sym::RuntimeConstant<double> a{17};
    sym::Exp<decltype(a)> exp{a};
    EXPECT_NO_THROW(sym::toString(exp));
}

TEST(Exp, Gradient) {
    sym::Variable<'x'> x;
    sym::Exp<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x = 1), std::exp(1));
}
