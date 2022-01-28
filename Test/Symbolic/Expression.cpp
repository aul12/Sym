#include <Symbolic/Expression.hpp>
#include <gtest/gtest.h>

TEST(Expression, NonCvRefTypeTrue) {
    EXPECT_TRUE(sym::nonCvRefType<int>);
}

TEST(Expression, NonCvRefTypeConst) {
    EXPECT_FALSE(sym::nonCvRefType<const int>);
}

TEST(Expression, NonCvRefTypeRef) {
    EXPECT_FALSE(sym::nonCvRefType<int &>);
}

TEST(Expression, NonCvRefTypeConstRef) {
    EXPECT_FALSE(sym::nonCvRefType<const int &>);
}

TEST(Expression, AllCvRefTypeTrue) {
    EXPECT_TRUE(sym::AllNonCvRefType(std::tuple<int, int>()));
}

TEST(Expression, AllCvRefTypeOne) {
    int a;
    EXPECT_FALSE(sym::AllNonCvRefType(std::tuple<int, int &>(a, a)));
}

TEST(Expression, AllCvRefTypeAll) {
    int a;
    EXPECT_FALSE(sym::AllNonCvRefType(std::tuple<int &, int &>(a, a)));
}
