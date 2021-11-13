#include "Symbolic/Variable.hpp"

#include <gtest/gtest.h>

TEST(Variable, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::Variable<int, 'a'>>::val));
}

TEST(Variable, Resolve) {
    sym::Variable<int, 'a'> a{42};
    EXPECT_EQ(a.resolve(), 42);
}

TEST(Variable, GradEqual) {
    sym::Variable<int, 'a'> a{42};
    EXPECT_EQ(sym::gradient(a, a).resolve(), 1);
}

TEST(Variable, GradNEqual) {
    sym::Variable<int, 'a'> a{42};
    sym::Variable<int, 'b'> b{42};
    EXPECT_EQ(sym::gradient(a, b).resolve(), 0);
}
