#include <Symbolic/Operations/Greater.hpp>
#include <Symbolic/Variable.hpp>
#include <gtest/gtest.h>

TEST(Greater, IsExpression) {
    EXPECT_TRUE((sym::IsExpression<sym::Greater<sym::Variable<'a'>, sym::Variable<'b'>>>::val));
}

TEST(Greater, Resolve) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Greater aGTb{a, b};
    EXPECT_TRUE(aGTb.resolve(a=1, b=0));
    EXPECT_FALSE(aGTb.resolve(a=-1, b=0));
}
