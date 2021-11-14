/**
 * @file Constant.cpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */

#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(RuntimeConstant, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::RuntimeConstant<int>>::val));
}

TEST(RuntimeConstant, Resolve) {
    sym::RuntimeConstant<int> c{42};

    EXPECT_EQ(c.resolve(), 42);
}

TEST(RuntimeConstant, GradResolve) {
    sym::RuntimeConstant<int> c{42};

    EXPECT_EQ(sym::gradient(c, sym::Variable<'a'>{}).resolve(), 0);
}
