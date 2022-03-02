#include <Symbolic/Operators.hpp>
#include <Symbolic/Simplifier/CompileTime.hpp>
#include <gtest/gtest.h>

TEST(Simplifier_CompileTime, AddMergeConstant) {
    sym::CompiletimeConstant<int, 1> a;
    sym::CompiletimeConstant<int, 2> b;
    auto sum = a + b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 3>>) );
}

TEST(Simplifier_CompileTime, AddMergeIdentityKeepLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = a + b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}

TEST(Simplifier_CompileTime, AddMergeIdentityKeepRhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = b + a;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}


TEST(Simplifier_CompileTime, SubMergeConstant) {
    sym::CompiletimeConstant<int, 1> a;
    sym::CompiletimeConstant<int, 2> b;
    auto sum = a - b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, -1>>) );
}

TEST(Simplifier_CompileTime, SubMergeIdentityKeepLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = a - b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}

TEST(Simplifier_CompileTime, MulMergeConstant) {
    sym::CompiletimeConstant<int, 1> a;
    sym::CompiletimeConstant<int, 2> b;
    auto sum = a * b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 2>>) );
}

TEST(Simplifier_CompileTime, MulMergeIdentityKeepLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 1> b;
    auto sum = a * b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}

TEST(Simplifier_CompileTime, MulMergeIdentityKeepRhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 1> b;
    auto sum = b * a;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}

TEST(Simplifier_CompileTime, MulMergeDisappearFromLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = b * a;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 0>>) );
}

TEST(Simplifier_CompileTime, MulMergeDisappearFromRhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = a * b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 0>>) );
}

TEST(Simplifier_CompileTime, DivMergeConstant) {
    sym::CompiletimeConstant<int, 1> a;
    sym::CompiletimeConstant<int, 2> b;
    auto sum = b / a;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 2>>) );
}

TEST(Simplifier_CompileTime, DivMergeIdentityKeepLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 1> b;
    auto sum = a / b;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), decltype(a)>) );
}

TEST(Simplifier_CompileTime, DivMergeDisappearFromLhs) {
    sym::Variable<'a'> a;
    sym::CompiletimeConstant<int, 0> b;
    auto sum = b / a;

    auto newSum = sym::simplifier::simplifyNodeCompileTime(sum);

    EXPECT_TRUE((std::is_same_v<decltype(newSum), sym::CompiletimeConstant<int, 0>>) );
}

TEST(Simplifier_CompileTime, TernaryBothEqual) {
    sym::CompiletimeConstant<int, 17> b;
    sym::Variable<'a'> a;
    auto tern = sym::Ternary{a, b, b};

    auto newTern = sym::simplifier::simplifyNodeCompileTime(tern);

    EXPECT_TRUE((std::is_same_v<decltype(newTern), sym::CompiletimeConstant<int, 17>>) );
}

TEST(Simplifier_CompileTime, TernaryCondConstant) {
    sym::CompiletimeConstant<int, 1> b;
    sym::Variable<'a'> a;
    auto tern = sym::Ternary{b, a, b*a};

    auto newTern = sym::simplifier::simplifyNodeCompileTime(tern);

    EXPECT_TRUE((std::is_same_v<decltype(newTern), sym::Variable<'a'>>) );
}
