#include "Symbolic/Vector.hpp"

#include <Eigen/Dense>
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

TEST(Vector, ToString) {
    sym::Variable<'a'> a;
    sym::Vector vec{a, a + a, a * a};
    EXPECT_NO_THROW(sym::toString(vec));
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
    EXPECT_EQ(std::get<1>(std::get<0>(res)), 1);
    EXPECT_EQ(std::get<0>(std::get<1>(res)), 17);
    EXPECT_EQ(std::get<1>(std::get<1>(res)), 1);
}

TEST(Vector, GradientsResolveAsEigen) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Vector f{a * b, a + b};
    auto grad = sym::gradient(f, sym::Vector{a, b});
    auto vec = grad.resolveAs<Eigen::Matrix<int, 4, 1>>(a = 17, b = 42);
    Eigen::Map<Eigen::Matrix<int, 2, 2>> res(vec.data(), 2, 2);
    EXPECT_EQ(res(0, 0), 42);
    EXPECT_EQ(res(0, 1), 17);
    EXPECT_EQ(res(1, 0), 1);
    EXPECT_EQ(res(1, 1), 1);
}

TEST(Vector, ResolveFromEigen) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::Variable<'c'> c;
    sym::Vector sym{a, b, c};
    Eigen::Vector3d vec{17, 38, 42};
    auto resolved = sym.resolve(sym::bindVectorFromContainer(sym, vec));
    EXPECT_EQ(std::get<0>(resolved), vec[0]);
    EXPECT_EQ(std::get<1>(resolved), vec[1]);
    EXPECT_EQ(std::get<2>(resolved), vec[2]);
}
