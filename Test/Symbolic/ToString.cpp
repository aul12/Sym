#include <gtest/gtest.h>

#include "Symbolic/Operators.hpp"
#include "Symbolic/Variable.hpp"

TEST(ToString, String) {
    sym::Variable<"acceleration"> v;
    EXPECT_EQ(toString(v), "acceleration");
}

TEST(ToString, Char) {
    sym::Variable<'a'> v;
    EXPECT_EQ(toString(v), "a");
}

TEST(ToString, Number) {
    sym::Variable<(std::size_t) 7> v;
    EXPECT_EQ(toString(v), "0000000000000007");

    sym::Variable<(std::size_t) 0xabcdef0123456789> v2;
    EXPECT_EQ(toString(v2), "abcdef0123456789");
}

TEST(ToString, IdentifierName) {
    SYM_VARIABLE(v);
    EXPECT_EQ(toString(v), "v");

    SYM_VARIABLE(longName);
    EXPECT_EQ(toString(longName), "longName");
}

TEST(ToString, AutomaticName) {
    SYM_A_VARIABLE v;
    EXPECT_TRUE(toString(v).ends_with("Test/Symbolic/ToString.cpp:" + std::to_string(__LINE__ - 1)));

    SYM_A_VARIABLE longName;
    EXPECT_TRUE(toString(longName).ends_with("Test/Symbolic/ToString.cpp:" + std::to_string(__LINE__ - 1)));

    EXPECT_NE(toString(v), toString(longName));
    EXPECT_FALSE((std::is_same_v<decltype(v), decltype(longName)>) );
}

TEST(ToString, DivideByRuntimeConstant) {
    SYM_VARIABLE(a);
    auto div = a / sym::RuntimeConstant(2.0);
    EXPECT_EQ(toString(div), "(a/2.000000)");
}
