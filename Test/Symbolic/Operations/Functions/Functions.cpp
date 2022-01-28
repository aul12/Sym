#include <Symbolic/Operations/Functions/Functions.hpp>
#include <gtest/gtest.h>

TEST(Functions, CosNoRef) {
    SYM_VARIABLE(a);

    auto c = std::cos(a);

    EXPECT_TRUE(sym::IsExpression<decltype(c)>::val);

    EXPECT_TRUE((std::is_same_v<decltype(c), sym::Cos<decltype(a)>>) );
}
