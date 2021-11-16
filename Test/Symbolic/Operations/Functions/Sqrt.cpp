#include <Symbolic/Operations/Functions/Sqrt.hpp>
#include <Symbolic/RuntimeConstant.hpp>
#include <gtest/gtest.h>

TEST(Sqrt, Expression) {
    using Var = sym::Variable<'x'>;
    EXPECT_TRUE((sym::IsExpression<sym::Sqrt<Var>>::val));
}

TEST(Sqrt, Resolve) {
    sym::RuntimeConstant<double> a{16};
    sym::Sqrt<decltype(a)> sqrt{a};
    EXPECT_DOUBLE_EQ(sqrt.resolve(), 4);
}

TEST(Sqrt, Gradient) {
    sym::Variable<'x'> x;
    sym::Sqrt<decltype(x)> y{x};
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x = 1), 0.5 * 1 / 1);
    EXPECT_DOUBLE_EQ(sym::gradient(y, x).resolve(x = 4), 0.5 * 1 / 2);
}
