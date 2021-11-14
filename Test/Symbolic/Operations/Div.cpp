#include "Symbolic/Operations/Div.hpp"

#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Div, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Div<Const, Const>>::val));
}

TEST(Div, Resolve) {
    sym::RuntimeConstant<int> a{42};
    sym::RuntimeConstant<int> b{2};
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(div.resolve(), 21);
}

TEST(Div, GradA) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_DOUBLE_EQ(sym::gradient(div, a).resolve(a=42.0, b=2), 0.5);
}

TEST(Div, GradB) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(sym::gradient(div, b).resolve(a=17, b=42), 17 / (42 * 42));
}

TEST(Div, GradNone) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Variable<'c'> c;
    sym::Div<decltype(a), decltype(b)> div{a, b};
    EXPECT_EQ(sym::gradient(div, c).resolve(a=17, b=42, c=42), 0);
}

TEST(Div, GradBoth) {
    sym::Variable<'a'> a;
    sym::Div<decltype(a), decltype(a)> div{a, a};
    EXPECT_EQ(sym::gradient(div, a).resolve(a=17), 0);
}
