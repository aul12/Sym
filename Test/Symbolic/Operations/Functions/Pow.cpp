#include "Symbolic/Operations/Functions/Pow.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Pow, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Pow<Const, Const>>::val));
}

TEST(Pow, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::RuntimeConstant<double> b{17};
    sym::Pow pow{a, b};
    EXPECT_DOUBLE_EQ(pow.resolve(), std::pow(0.5, 17));
}

TEST(Pow, ToString) {
    sym::RuntimeConstant<double> a{0.5};
    sym::RuntimeConstant<double> b{17};
    sym::Pow pow{a, b};
    EXPECT_NO_THROW(sym::toString(pow));
}

TEST(Pow, GradientA) {
    sym::Variable<"a"> a;
    sym::CompiletimeConstant<int, 2> b;
    sym::Pow y{a, b};
    EXPECT_DOUBLE_EQ(sym::gradient(y, a).resolve(a = 17), 34);
}

TEST(Pow, GradientB) {
    sym::RuntimeConstant<double> a{std::exp(1)};
    sym::Variable<"b"> b;
    sym::Pow y{a, b};
    EXPECT_DOUBLE_EQ(sym::gradient(y, b).resolve(b = 8), std::exp(8));
}
