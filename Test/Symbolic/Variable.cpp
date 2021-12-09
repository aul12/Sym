#include "Symbolic/Variable.hpp"

#include <gtest/gtest.h>

TEST(Variable, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::Variable<'a'>>::val));
}

TEST(Variable, Resolve) {
    sym::Variable<'a'> a;
    EXPECT_EQ(a.resolve(a = 42), 42);
}

TEST(Variable, ToString) {
    sym::Variable<'a'> a;
    EXPECT_NO_THROW(sym::toString(a));
}

TEST(Variable, GradEqual) {
    sym::Variable<'a'> a;
    auto grad = sym::gradient(a, a);
    EXPECT_EQ(grad.resolve(), 1);
}

TEST(Variable, GradNEqual) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    EXPECT_EQ(sym::gradient(a, b).resolve(a = 42), 0);
}
