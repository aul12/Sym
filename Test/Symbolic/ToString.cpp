#include <gtest/gtest.h>

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
