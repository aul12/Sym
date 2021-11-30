#include <Symbolic/Operators.hpp>
#include <Symbolic/RuntimeConstant.hpp>
#include <Symbolic/Simplifier/Runtime.hpp>
#include <Symbolic/Simplifier/Simplifier.hpp>
#include <Symbolic/Vector.hpp>
#include <gtest/gtest.h>

TEST(Simplifier_Runtime, Simple) {
    sym::RuntimeConstant a(1);
    sym::RuntimeConstant b(2);
    auto expr = (a + b);
    auto simplified = sym::simplifier::simplifyRuntime(expr);
    EXPECT_TRUE((std::is_same_v<sym::RuntimeConstant<int>, decltype(simplified)>) );
    EXPECT_EQ(simplified.resolve(), 3);
}

TEST(Simplifier_Runtime, Vector) {
    sym::RuntimeConstant a(1);
    sym::RuntimeConstant b(2);
    sym::Vector vec{a * b, a + b};
    auto simplified = sym::simplifier::simplify(vec);
    auto children = sym::getChildren(simplified);
    EXPECT_TRUE((std::is_same_v<std::remove_cvref_t<decltype(std::get<0>(children))>, sym::RuntimeConstant<int>>) );
    EXPECT_TRUE((std::is_same_v<std::remove_cvref_t<decltype(std::get<1>(children))>, sym::RuntimeConstant<int>>) );
    auto resolved = simplified.resolveAs<std::array<int, 2>>();
    EXPECT_EQ(resolved[0], 2);
    EXPECT_EQ(resolved[1], 3);
}
