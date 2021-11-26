#include <Symbolic/Operators.hpp>
#include <Symbolic/RuntimeConstant.hpp>
#include <Symbolic/Simplifier/Runtime.hpp>
#include <gtest/gtest.h>

TEST(Simplifier_Runtime, Simple) {
    sym::RuntimeConstant a(1);
    sym::RuntimeConstant b(2);
    auto expr = (a + b);
    auto simplified = sym::simplifier::simplifyRuntime(expr);
    EXPECT_TRUE((std::is_same_v<sym::RuntimeConstant<int>, decltype(simplified)>) );
    EXPECT_EQ(simplified.resolve(), 3);
}
