#include "Symbolic/Operations/Add.hpp"

#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(Add, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::Add<Const, Const>>::val));
}

TEST(Add, Resolve) {
    sym::RuntimeConstant<int> a{17};
    sym::RuntimeConstant<int> b{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(add.resolve(), 17 + 42);
}

TEST(Add, ToString) {
    sym::RuntimeConstant<int> a{17};
    sym::RuntimeConstant<int> b{42};
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_NO_THROW(sym::toString(add));
}

TEST(Add, GradA) {
    sym::Variable<"a"> a;
    sym::Variable<"b"> b;
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(add, a).resolve(a=17, b=42), 1);
}

TEST(Add, GradB) {
    sym::Variable<"a"> a;
    sym::Variable<"b"> b;
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(add, b).resolve(a=17, b=42), 1);
}

TEST(Add, GradNone) {
    sym::Variable<"a"> a;
    sym::Variable<"b"> b;
    sym::Variable<"c"> c;
    sym::Add<decltype(a), decltype(b)> add{a, b};
    EXPECT_EQ(sym::gradient(add, c).resolve(a=17, b=42, c=42), 0);
}

TEST(Add, GradBoth) {
    sym::Variable<"a"> a;
    sym::Add<decltype(a), decltype(a)> add{a, a};
    EXPECT_EQ(sym::gradient(add, a).resolve(a=17), 2);
}
