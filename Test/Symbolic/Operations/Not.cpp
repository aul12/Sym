#include "Symbolic/Operations/Not.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Variable.hpp"

TEST(Not, IsExpression) {
    EXPECT_TRUE(sym::IsExpression<sym::Not<sym::Variable<'a'>>>::val);
}

TEST(Not, Resolve) {
    sym::Variable<'a'> a;
    sym::Not notA{a};
    EXPECT_TRUE(notA.resolve(a=false));
    EXPECT_FALSE(notA.resolve(a=true));
}
