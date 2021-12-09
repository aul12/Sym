#include "Symbolic/Operations/Ternary.hpp"
#include "Symbolic/Operators.hpp"

#include <gtest/gtest.h>

TEST(Ternary, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::Ternary<sym::Variable<'a'>, sym::Variable<'a'>, sym::Variable<'a'>>>::val));
}

TEST(Ternary, Resolve) {
    sym::Variable<'c'> c;
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Ternary ternary{c, a, b};
    EXPECT_EQ(ternary.resolve(c=true, a=1, b=2), 1);
    EXPECT_EQ(ternary.resolve(c=false, a=1, b=2), 2);
}

TEST(Ternary, ToString) {
    sym::Variable<'c'> c;
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Ternary ternary{c, a, b};
    EXPECT_NO_THROW(sym::toString(ternary));
}

TEST(Ternary, GradientTrue) {
    sym::Variable<'c'> c;
    sym::Variable<'a'> a;
    sym::Ternary ternary{c, a, a*2};
    auto grad = sym::gradient(ternary, a);
    EXPECT_EQ(grad.resolve(c=true, a=1), 1);
}

TEST(Ternary, GradientFalse) {
    sym::Variable<'c'> c;
    sym::Variable<'a'> a;
    sym::Ternary ternary{c, a, a*2};
    auto grad = sym::gradient(ternary, a);
    EXPECT_EQ(grad.resolve(c=false, a=1), 2);
}
