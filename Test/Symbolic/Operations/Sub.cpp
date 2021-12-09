#include "Symbolic/Operations/Sub.hpp"

#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Sub, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Sub<Const, Const>>::val));
}

TEST(Sub, Resolve) {
    sym::RuntimeConstant<int> a{17};
    sym::RuntimeConstant<int> b{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sub.resolve(), 17 - 42);
}

TEST(Sub, ToString) {
    sym::RuntimeConstant<int> a{17};
    sym::RuntimeConstant<int> b{42};
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_NO_THROW(sym::toString(sub));
}

TEST(Sub, GradA) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(sub, a).resolve(a = 17, b = 42), 1);
}

TEST(Sub, GradB) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(sub, b).resolve(a = 17, b = 42), -1);
}

TEST(Sub, GradNone) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Variable<'c'> c;
    sym::Sub<decltype(a), decltype(b)> sub{a, b};
    EXPECT_EQ(sym::gradient(sub, c).resolve(a = 17, b = 42, c = 42), 0);
}

TEST(Sub, GradBoth) {
    sym::Variable<'a'> a;
    sym::Sub<decltype(a), decltype(a)> sub{a, a};
    EXPECT_EQ(sym::gradient(sub, a).resolve(a = 17), 0);
}
