/**
 * @file Simplifier.hpp
 * @author paul
 * @date 14.11.21
 * Description here TODO
 */
#ifndef SYM_SIMPLIFIER_HPP
#define SYM_SIMPLIFIER_HPP

#include "../CompiletimeConstant.hpp"
#include "../Expression.hpp"
#include "../Operations/Add.hpp"

namespace sym::simplifier {
    namespace impl {
        template<Expression Expr>
        struct IsConstant {
            static constexpr auto val = false;
        };

        template<typename T, T val_>
        struct IsConstant<CompiletimeConstant<T, val_>> {
            static constexpr auto val = true;
        };

        template<Expression Expr>
        constexpr auto isConstant = IsConstant<Expr>::val;
    } // namespace impl

    template<typename T>
    struct MergeConstant {
        static constexpr bool exists = false;
    };

#define CONSTANT_MERGER(OpName, Op)                                                                                    \
    template<Expression Lhs, Expression Rhs>                                                                           \
    requires(impl::isConstant<Lhs> and impl::isConstant<Rhs>) struct MergeConstant<OpName<Lhs, Rhs>> {                 \
        static constexpr bool exists = true;                                                                           \
        static constexpr auto val = Lhs::resolve() Op Rhs::resolve();                                                  \
        static constexpr CompiletimeConstant<std::remove_const_t<decltype(val)>, val> newVal{};                        \
    };

    CONSTANT_MERGER(Add, +)
    CONSTANT_MERGER(Sub, -)
    CONSTANT_MERGER(Mul, *)
    CONSTANT_MERGER(Div, /)

    enum class MergeStatus { NONE, KEEP_LHS, KEEP_RHS };

    template<typename T>
    struct MergeIdentity {
        static constexpr MergeStatus status = MergeStatus::NONE;
    };

#define IDENTITY_MERGE_KEEP_LHS(OpName, identityVal)                                                                   \
    template<Expression Lhs, Expression Rhs>                                                                           \
    requires(impl::isConstant<Rhs> and Rhs::resolve() == identityVal) struct MergeIdentity<OpName<Lhs, Rhs>> {         \
        static constexpr MergeStatus status = MergeStatus::KEEP_LHS;                                                   \
    };

#define IDENTITY_MERGE_KEEP_RHS(OpName, identityVal)                                                                   \
    template<Expression Lhs, Expression Rhs>                                                                           \
    requires(impl::isConstant<Lhs> and Lhs::resolve() == identityVal) struct MergeIdentity<OpName<Lhs, Rhs>> {         \
        static constexpr MergeStatus status = MergeStatus::KEEP_RHS;                                                   \
    };


    IDENTITY_MERGE_KEEP_LHS(Add, 0)
    IDENTITY_MERGE_KEEP_LHS(Sub, 0)
    IDENTITY_MERGE_KEEP_LHS(Mul, 1)
    IDENTITY_MERGE_KEEP_LHS(Div, 1)

    IDENTITY_MERGE_KEEP_RHS(Add, 0)
    IDENTITY_MERGE_KEEP_RHS(Mul, 1)

    template<typename T>
    struct MergeDisappear {
        static constexpr auto exists = false;
    };

    template<Expression Lhs, Expression Rhs>
    requires(impl::isConstant<Lhs> and Lhs::resolve() == 0) struct MergeDisappear<Mul<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Lhs newVal = Lhs{};
    };

    template<Expression Lhs, Expression Rhs>
    requires(impl::isConstant<Rhs> and Rhs::resolve() == 0) struct MergeDisappear<Mul<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Rhs newVal = Rhs{};
    };

    template<Expression Lhs, Expression Rhs>
    requires(impl::isConstant<Lhs> and Lhs::resolve() == 0) struct MergeDisappear<Div<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Lhs newVal = Lhs{};
    };

    template<Expression Expr>
    constexpr auto simplifyNodeCompileTime(const Expr &expr) {
        if constexpr (MergeConstant<Expr>::exists) {
            return simplifyNodeCompileTime(MergeConstant<Expr>::newVal);
        } else if constexpr (MergeDisappear<Expr>::exists) {
            return simplifyNodeCompileTime(MergeDisappear<Expr>::newVal);
        } else if constexpr (MergeIdentity<Expr>::status != MergeStatus::NONE) {
            auto children = getChildren(expr);
            if constexpr (MergeIdentity<Expr>::status == MergeStatus::KEEP_LHS) {
                return simplifyNodeCompileTime(std::get<0>(children));
            } else {
                return simplifyNodeCompileTime(std::get<1>(children));
            }
        } else {
            return expr;
        }
    }


} // namespace sym

#endif // SYM_SIMPLIFIER_HPP
