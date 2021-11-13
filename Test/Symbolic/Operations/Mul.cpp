#include "Symbolic/Operations/Mul.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Mul, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Mul<Const, Const>>::val));
}

TEST(Mul, Resolve) {
    sym::Constant<int> a{17};
    sym::Constant<int> b{42};
    sym::Mul<decltype(a), decltype(b)> mul{a, b};
    EXPECT_EQ(mul.resolve(), 17 * 42);
}

TEST(Mul, GradA) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Mul<decltype(a), decltype(b)> mul{a, b};
    EXPECT_EQ(sym::gradient(mul, a).resolve(), 42);
}

TEST(Mul, GradB) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Mul<decltype(a), decltype(b)> mul{a, b};
    EXPECT_EQ(sym::gradient(mul, b).resolve(), 17);
}

TEST(Mul, GradNone) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Variable<int, 'c'> c{42};
    sym::Mul<decltype(a), decltype(b)> mul{a, b};
    EXPECT_EQ(sym::gradient(mul, c).resolve(), 0);
}

TEST(Mul, GradBoth) {
    sym::Variable<int, 'a'> a{17};
    sym::Mul<decltype(a), decltype(a)> mul{a, a};
    EXPECT_EQ(sym::gradient(mul, a).resolve(), 2 * 17);
}
