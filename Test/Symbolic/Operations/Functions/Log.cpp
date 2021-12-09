#include "Symbolic/Operations/Functions/Log.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Log, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Log<Const>>::val));
}

TEST(Log, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::Log<decltype(a)> log{a};
    EXPECT_DOUBLE_EQ(log.resolve(), std::log(0.5));
}

TEST(Log, ToString) {
    sym::RuntimeConstant<double> a{0.5};
    sym::Log<decltype(a)> log{a};
    EXPECT_NO_THROW(sym::toString(log));
}

TEST(Log, Gradient) {
    sym::Variable<'x'> x;
    sym::Log<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x=0.5), 2);
}
