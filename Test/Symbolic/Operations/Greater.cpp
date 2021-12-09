#include <Symbolic/Operations/Greater.hpp>
#include <Symbolic/Variable.hpp>
#include <gtest/gtest.h>

TEST(Greater, IsExpression) {
    EXPECT_TRUE((sym::IsExpression<sym::Greater<sym::Variable<'a'>, sym::Variable<'b'>>>::val));
}

TEST(Greater, ResolveTrue) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Greater aGTb{a, b};
    EXPECT_TRUE(aGTb.resolve(a = 1, b = 0));
}

TEST(Greater, ResolveFalse) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Greater aGTb{a, b};
    EXPECT_FALSE(aGTb.resolve(a = -1, b = 0));
}

TEST(Greater, ToString) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Greater aGTb{a, b};
    EXPECT_NO_THROW(sym::toString(aGTb));
}
