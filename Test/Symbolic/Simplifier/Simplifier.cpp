#include "Symbolic/Simplifier/Simplifier.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Operators.hpp"

TEST(Simplifier, Complete) {
    sym::Variable<'a'> a;
    sym::Variable<'b'> b;
    sym::CompiletimeConstant<int, 0> zero;
    sym::CompiletimeConstant<int, 1> one;
    auto complex = ((a + zero) * one) + (b / one + b * zero);
    auto manual = a + b;
    auto simplified = sym::simplifier::simplifiy(complex);
    EXPECT_TRUE((std::is_same_v<decltype(manual), decltype(simplified)>) );
}
