#include "Symbolic/Operations/Add.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Constant.hpp"

TEST(Add, Expression) {
    using Const = sym::Constant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Add<Const, Const>>::val));
}

TEST(Add, Resolve) {
    sym::Constant<int> a{17};
    sym::Constant<int> b{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(add.resolve(), 17 + 42);
}

TEST(Add, GradA) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(a, a).resolve(), 1);
}

TEST(Add, GradB) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(add, b).resolve(), 1);
}

TEST(Add, GradNone) {
    sym::Variable<int, 'a'> a{17};
    sym::Variable<int, 'b'> b{42};
    sym::Variable<int, 'c'> c{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(add, c).resolve(), 0);
}

TEST(Add, GradBoth) {
    sym::Variable<int, 'a'> a{17};
    sym::Add<decltype(a), decltype(a)> add{a, a};
    EXPECT_EQ(sym::gradient(add, a).resolve(), 2);
}
