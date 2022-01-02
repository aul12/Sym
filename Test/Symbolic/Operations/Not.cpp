#include "Symbolic/Operations/Not.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Variable.hpp"

TEST(Not, IsExpression) {
    EXPECT_TRUE(sym::IsExpression<sym::Not<sym::Variable<"a">>>::val);
}

TEST(Not, ResolveTrue) {
    sym::Variable<"a"> a;
    sym::Not notA{a};
    EXPECT_TRUE(notA.resolve(a = false));
}

TEST(Not, ResolveFalse) {
    sym::Variable<"a"> a;
    sym::Not notA{a};
    EXPECT_FALSE(notA.resolve(a = true));
}

TEST(Not, ToString) {
    sym::Variable<"a"> a;
    sym::Not notA{a};
    EXPECT_NO_THROW(sym::toString(notA));
}
