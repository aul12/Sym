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
    auto result = vec.resolve(a = 42);
    EXPECT_EQ(std::get<0>(result), 42);
    EXPECT_EQ(std::get<1>(result), 42 + 42);
    EXPECT_EQ(std::get<2>(result), 42 * 42);
}

TEST(Vector, ResolveTo) {
    sym::Variable<'a'> a;
    sym::Vector vec{a, a + a, a * a};
    auto res = vec.resolveAs<std::vector<int>>(a = 42);
    EXPECT_EQ(res[0], 42);
    EXPECT_EQ(res[1], 42 + 42);
    EXPECT_EQ(res[2], 42 * 42);
}

TEST(Vector, Gradient) {
    sym::Variable<'a'> a;
    sym::Vector vec{a, a + a, a * a};
    auto dvec = sym::gradient(vec, a);
    auto res = dvec.resolveAs<std::vector<int>>(a = 42);
    EXPECT_EQ(res[0], 1);
    EXPECT_EQ(res[1], 2);
    EXPECT_EQ(res[2], 2 * 42);
}

TEST(Vector, Gradients) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Vector f{a * b, a + b};
    auto grad = sym::gradient(f, sym::Vector{a, b});
    auto res = grad.resolve(a = 17, b = 42);
    EXPECT_EQ(std::get<0>(std::get<0>(res)), 42);
    EXPECT_EQ(std::get<0>(std::get<1>(res)), 1);
    EXPECT_EQ(std::get<1>(std::get<0>(res)), 17);
    EXPECT_EQ(std::get<1>(std::get<1>(res)), 1);
}

TEST(Vector, GradientsResolveAs) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Vector f{a * b, a + b};
    auto grad = sym::gradient(f, sym::Vector{a, b});
    auto res = grad.resolveAs<std::vector<int>>(a = 17, b = 42);
    EXPECT_EQ(res[0], 42);
    EXPECT_EQ(res[1], 17);
    EXPECT_EQ(res[2], 1);
    EXPECT_EQ(res[3], 1);
}
