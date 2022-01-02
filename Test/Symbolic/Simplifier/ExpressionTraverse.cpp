#include "Symbolic/Simplifier/ExpressionTraverse.hpp"

#include <gtest/gtest.h>

#include "Symbolic/Operators.hpp"
#include "Symbolic/Variable.hpp"

TEST(Simplifier_Traverse, Enter) {
    sym::Variable<"a"> a;
    sym::Variable<"b"> b;
    auto expr = a + b * (a - b) + b * 3;
    auto traversed = sym::simplifier::traverseExpression(
            expr, [](auto && /*expr*/) { return sym::CompiletimeConstant<int, 0>{}; },
            [](auto &&expr) { return expr; });

    EXPECT_TRUE((std::is_same_v<decltype(traversed), sym::CompiletimeConstant<int, 0>>) );
}

TEST(Simplifier_Traverse, Leave) {
    sym::Variable<"a"> a;
    sym::Variable<"b"> b;
    auto expr = a + b * (a - b) + b * 3;
    auto traversed = sym::simplifier::traverseExpression(
            expr, [](auto &&expr) { return expr; },
            [](auto && /*expr*/) { return sym::CompiletimeConstant<int, 0>{}; });
    EXPECT_TRUE((std::is_same_v<decltype(traversed), sym::CompiletimeConstant<int, 0>>) );
}
