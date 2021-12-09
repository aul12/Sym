/**
 * @file Constant.cpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */

#include <gtest/gtest.h>

#include "Symbolic/CompiletimeConstant.hpp"

TEST(CompiletimeConstant, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::CompiletimeConstant<int, 42>>::val));
}

TEST(CompiletimeConstant, Resolve) {
    sym::CompiletimeConstant<int, 42> c;
    EXPECT_EQ(c.resolve(), 42);
}

TEST(CompiletimeConstant, ToString) {
    sym::CompiletimeConstant<int, 42> c;
    EXPECT_NO_THROW(sym::toString(c));
}

TEST(CompiletimeConstant, GradResolve) {
    sym::CompiletimeConstant<int, 42> c;

    EXPECT_EQ(sym::gradient(c, sym::Variable<'a'>{}).resolve(), 0);
}
