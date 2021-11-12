#include "Symbolic/Variable.hpp"

#include <gtest/gtest.h>

TEST(Variable, Expression) {
    EXPECT_TRUE(sym::IsExpression<sym::Variable<int>>::val);
}

TEST(Variable, Resolve) {
    sym::Variable<int> a{42};
    EXPECT_EQ(a.resolve(), 42);
}

TEST(Variable, Set) {
    sym::Variable<int> a{42};
    a.set(17);
    EXPECT_EQ(a.resolve(), 17);
}

TEST(Variable, GradEqual) {
    sym::Variable<int> a{42};
    EXPECT_EQ(sym::gradient(a, a).resolve(), 1);
}

TEST(Variable, GradNEqual) {
    sym::Variable<int> a{42};
    sym::Variable<int> b{42};
    EXPECT_EQ(sym::gradient(a, b).resolve(), 0);
}

TEST(Variable, IsConstant) {
    EXPECT_FALSE(sym::Variable<int>::isConstant());
}
