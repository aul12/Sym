#include "Symbolic/Vector.hpp"

#include <Symbolic/Operators.hpp>
#include <Symbolic/Variable.hpp>
#include <gtest/gtest.h>

TEST(Vector, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::Vector<>>::val));
}

TEST(Vector, Resolve) {
    sym::Variable<'a'> a;
    sym::Vector vec{a, a + a, a * a};
    auto result = vec.resolve(a=42);
    EXPECT_EQ(std::get<0>(result), 42);
    EXPECT_EQ(std::get<1>(result), 42+42);
    EXPECT_EQ(std::get<2>(result), 42*42);
}

TEST(Vector, ResolveTo) {
    sym::Variable<'a'> a;
    sym::Vector vec{a, a + a, a * a};
    auto res = vec.resolveAs<std::vector<int>>(a=42);
    EXPECT_EQ(res[0], 42);
    EXPECT_EQ(res[1], 42+42);
    EXPECT_EQ(res[2], 42*42);
}
