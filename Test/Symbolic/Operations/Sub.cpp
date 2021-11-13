#include "Symbolic/Sub.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Sub, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Sub<Const, Const>>::val));
}

TEST(Sub, Resolve) {
    sym::Constant<int> a{17};
    sym::Constant<int> b{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sub.resolve(), 17 - 42);
}

TEST(Sub, GradA) {
    sym::Variable<int> a{17};
    sym::Variable<int> b{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(a, a).resolve(), 1);
}

TEST(Sub, GradB) {
    sym::Variable<int> a{17};
    sym::Variable<int> b{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(sub, b).resolve(), -1);
}

TEST(Sub, GradNone) {
    sym::Variable<int> a{17};
    sym::Variable<int> b{42};
    sym::Variable<int> c{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(sub, c).resolve(), 0);
}

TEST(Sub, GradBoth) {
    sym::Variable<int> a{17};
    sym::Sub<decltype(a), decltype(a)> sub{a, a};
    EXPECT_EQ(sym::gradient(sub, a).resolve(), 0);
}

TEST(Sub, IsConstantCC) {
    using C = sym::Constant<int>;
    EXPECT_TRUE((sym::Sub<C, C>::isConstant()));
}

TEST(Sub, IsConstantCV) {
    using C = sym::Constant<int>;
    using V = sym::Variable<int>;
    EXPECT_FALSE((sym::Sub<C, V>::isConstant()));
}

TEST(Sub, IsConstantVC) {
    using C = sym::Constant<int>;
    using V = sym::Variable<int>;
    EXPECT_FALSE((sym::Sub<V, C>::isConstant()));
}

TEST(Sub, IsConstantVV) {
    using V = sym::Variable<int>;
    EXPECT_FALSE((sym::Sub<V, V>::isConstant()));
}
