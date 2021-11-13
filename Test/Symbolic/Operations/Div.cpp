#include "Symbolic/Operations/Div.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Div, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Div<Const, Const>>::val));
}

TEST(Div, Resolve) {
    sym::Constant<int> a{42};
    sym::Constant<int> b{2};
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(div.resolve(), 21);
}

TEST(Div, GradA) {
    sym::Variable<double, 'a'> a{42};
    sym::Variable<double, 'b'> b{2};
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_DOUBLE_EQ(sym::gradient(div, a).resolve(), 0.5);
}

TEST(Div, GradB) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(sym::gradient(div, b).resolve(), 17 / (42 * 42));
}

TEST(Div, GradNone) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Variable<int, 'c'> c{42};
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(sym::gradient(div, c).resolve(), 0);
}

TEST(Div, GradBoth) {
    sym::Variable<int, 'a'> a{17};
    sym::Div<decltype(a), decltype(a)> div{a, a};
    EXPECT_EQ(sym::gradient(div, a).resolve(), 0);
}
