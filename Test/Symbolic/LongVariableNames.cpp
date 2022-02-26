#include <gtest/gtest.h>

#include "Symbolic/Operators.hpp"
#include "Symbolic/Simplifier/Simplifier.hpp"

TEST(LongVariableNames, DifferentLength) {
    sym::Variable<"acceleration"> acc;
    sym::Variable<"time"> time;
    auto acc_time = acc * time;
    auto g = sym::simplifier::simplify(sym::gradient(acc_time, time));
    EXPECT_EQ(g.resolve(acc = 7, time = 13), 7);
}

TEST(LongVariableNames, StringChar) {
    sym::Variable<"acceleration"> acc;
    sym::Variable<'t'> time;
    auto acc_time = acc * time;
    auto g = sym::simplifier::simplify(sym::gradient(acc_time, time));
    EXPECT_EQ(g.resolve(acc = 7, time = 13), 7);
}


TEST(LongVariableNames, SameLength) {
    sym::Variable<"acc"> acc;
    sym::Variable<"tim"> time;
    auto acc_time = acc * time;
    auto g = sym::simplifier::simplify(sym::gradient(acc_time, time));
    EXPECT_EQ(g.resolve(acc = 7, time = 13), 7);
}

TEST(LongVariableNames, SameName) {
    sym::Variable<"acc"> acc;
    sym::Variable<"acc"> time;                                         // This error is on purpose!
    auto acc_time = acc * time;                                        // This turns out as acc * acc
    auto g = sym::simplifier::simplify(sym::gradient(acc_time, time)); // This becomes 2 * acc
    EXPECT_EQ(g.resolve(acc = 7), 14);
}
