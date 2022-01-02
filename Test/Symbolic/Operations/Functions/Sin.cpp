#include "Symbolic/Operations/Functions/Sin.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Sin, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Sin<Const>>::val));
}

TEST(Sin, Resolve) {
    sym::RuntimeConstant<double> a{17};
    sym::Sin<decltype(a)> sin{a};
    EXPECT_DOUBLE_EQ(sin.resolve(), std::sin(17));
}

TEST(Sin, ToString) {
    sym::RuntimeConstant<double> a{17};
    sym::Sin<decltype(a)> sin{a};
    EXPECT_NO_THROW(sym::toString(sin));
}

TEST(Sin, Gradient) {
    sym::Variable<"a"> x;
    sym::Sin<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0), 1);
}
