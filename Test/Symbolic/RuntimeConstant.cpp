/**
 * @file Constant.cpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */

#include "Symbolic/RuntimeConstant.hpp"

#include <gtest/gtest.h>

TEST(RuntimeConstant, Expression) {
    EXPECT_TRUE((sym::IsExpression<sym::RuntimeConstant<int>>::val));
}

TEST(RuntimeConstant, Resolve) {
    sym::RuntimeConstant<int> c{42};

    EXPECT_EQ(c.resolve(), 42);
}

TEST(RuntimeConstant, ToString) {
    sym::RuntimeConstant<int> c{42};

    EXPECT_NO_THROW(sym::toString(c));
}

TEST(RuntimeConstant, GradResolve) {
    sym::RuntimeConstant<int> c{42};

    EXPECT_EQ(sym::gradient(c, sym::Variable<"a">{}).resolve(), 0);
}
