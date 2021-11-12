#include <Integration/Euler.hpp>
#include <gtest/gtest.h>

TEST(Euler, Constant) {
    constexpr auto C = 17;
    auto f = [C](auto) { return C; };

    constexpr auto min = 0.0;
    constexpr auto max = 0.5;
    constexpr auto delta = 0.25;
    constexpr auto steps = sym::integrate::helper::getSteps(min, max, delta);

    auto euler = sym::integrate::make_euler.make<steps>(f, min, delta);

    EXPECT_DOUBLE_EQ(euler.resolve(), (max - min) * C);
}

TEST(Euler, Identity) {
    auto f = [](auto x) { return x; };

    constexpr auto min = 0.0;
    constexpr auto max = 0.5;
    constexpr auto delta = 1.0 / 128;
    constexpr auto steps = sym::integrate::helper::getSteps(min, max, delta);

    auto euler = sym::integrate::make_euler.make<steps>(f, min, delta);

    EXPECT_NEAR(euler.resolve(), 0.5 * (max * max - min * min), 0.01);
}

TEST(Euler, IdentitySymmetric) {
    auto f = [](auto x) { return x; };

    constexpr auto min = -0.5;
    constexpr auto max = 0.5;
    constexpr auto delta = 1.0 / 64;
    constexpr auto steps = sym::integrate::helper::getSteps(min, max, delta);

    auto euler = sym::integrate::make_euler.make<steps>(f, min, delta);

    EXPECT_NEAR(euler.resolve(), 0.5 * (max * max - min * min), 0.01);
}

TEST(Euler, Square) {
    auto f = [](auto x) { return 3 * x * x; };

    constexpr auto min = 0.0;
    constexpr auto max = 0.5;
    constexpr auto delta = 1.0 / 128;
    constexpr auto steps = sym::integrate::helper::getSteps(min, max, delta);

    auto euler = sym::integrate::make_euler.make<steps>(f, min, delta);

    EXPECT_NEAR(euler.resolve(), (max * max * max - min * min * min), 0.01);
}
