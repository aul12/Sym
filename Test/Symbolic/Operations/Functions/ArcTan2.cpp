#include "Symbolic/Operations/Functions/ArcTan2.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include "Symbolic/RuntimeConstant.hpp"

TEST(ArcTan2, Expression) {
    using Const = sym::RuntimeConstant<int>;
    EXPECT_TRUE((sym::IsExpression<sym::ArcTan2<Const, Const>>::val));
}

TEST(ArcTan2, Resolve) {
    sym::RuntimeConstant<double> a{0.5};
    sym::RuntimeConstant<double> b{-17};
    sym::ArcTan2 atan{a, b};
    EXPECT_DOUBLE_EQ(atan.resolve(), std::atan2(0.5, -17));
}

TEST(ArcTan2, ToString) {
    sym::RuntimeConstant<double> a{0.5};
    sym::ArcTan2 atan{a, a};
    EXPECT_NO_THROW(sym::toString(atan));
}
