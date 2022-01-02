#include "Symbolic/Operations/Functions/Cos.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Cos, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Cos<Const>>::val));
}

TEST(Cos, Resolve) {
    sym::RuntimeConstant<double> a{17};
    sym::Cos<decltype(a)> cos{a};
    EXPECT_DOUBLE_EQ(cos.resolve(), std::cos(17));
}

TEST(Cos, ToString) {
    sym::RuntimeConstant<double> a{17};
    sym::Cos<decltype(a)> cos{a};
    EXPECT_NO_THROW(sym::toString(cos));
}

TEST(Cos, Gradient) {
    sym::Variable<"x"> x;
    sym::Cos<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 0);
}
