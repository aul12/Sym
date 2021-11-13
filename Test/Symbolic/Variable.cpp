#include "Symbolic/Variable.hpp"

#include <gtest/gtest.h>

TEST(Variable, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::Variable<'a'>>::val));
}

TEST(Variable, Resolve) {
    sym::Variable<'a'> a;
    EXPECT_EQ(a.resolve(a=42), 42);
}

TEST(Variable, GradEqual) {
    sym::Variable<'a'> a;
    EXPECT_EQ(sym::gradient(a, a).resolve(), 1);
}

TEST(Variable, GradNEqual) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    EXPECT_EQ(sym::gradient(a, b).resolve(a=42), 0);
}
