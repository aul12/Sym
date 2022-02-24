/**
 * @file CompileTime.hpp
 * @author paul
 * @date 14.11.21
 * Description here TODO
 */
#ifndef SYM_COMPILETIME_HPP
#define SYM_COMPILETIME_HPP

#include "../CompiletimeConstant.hpp"
#include "../Expression.hpp"
#include "../Operations/Add.hpp"
#include "../Operations/Div.hpp"
#include "../Operations/Mul.hpp"
#include "../Operations/Sub.hpp"
#include "Util.hpp"


namespace sym::simplifier {
    /*template<typename T>
    struct MergeConstant {
        static constexpr bool exists = false;
    };

#define CONSTANT_MERGER(OpName, Op)                                                                                    \
    template<Expression Lhs, Expression Rhs>                                                                           \
    requires(util::isCompileTimeConstant<Lhs> and                                                                      \
             util::isCompileTimeConstant<Rhs>) struct MergeConstant<OpName<Lhs, Rhs>> {                                \
        static constexpr bool exists = true;                                                                           \
        static constexpr auto val = Lhs::resolve() Op Rhs::resolve();                                                  \
        static constexpr CompiletimeConstant<std::remove_cvref_t<decltype(val)>, val> newVal{};                        \
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
    requires(Rhs::resolve() == identityVal) struct MergeIdentity<OpName<Lhs, Rhs>> {                                   \
        static constexpr MergeStatus status = MergeStatus::KEEP_LHS;                                                   \
    };

#define IDENTITY_MERGE_KEEP_RHS(OpName, identityVal)                                                                   \
    template<Expression Lhs, Expression Rhs>                                                                           \
    requires(Lhs::resolve() == identityVal) struct MergeIdentity<OpName<Lhs, Rhs>> {                                   \
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
    requires(Lhs::resolve() == 0) struct MergeDisappear<Mul<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Lhs newVal = Lhs{};
    };

    template<Expression Lhs, Expression Rhs>
    requires(Rhs::resolve() == 0) struct MergeDisappear<Mul<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Rhs newVal = Rhs{};
    };

    template<Expression Lhs, Expression Rhs>
    requires(Lhs::resolve() == 0) struct MergeDisappear<Div<Lhs, Rhs>> {
        static constexpr auto exists = true;
        static constexpr Lhs newVal = Lhs{};
    };

    template<Expression Expr>
    constexpr auto simplifyNodeCompileTime(Expr expr) {
        if constexpr (MergeConstant<Expr>::exists) {
            return MergeConstant<Expr>::newVal;
        } else if constexpr (MergeDisappear<Expr>::exists) {
            return MergeDisappear<Expr>::newVal;
        } else if constexpr (MergeIdentity<Expr>::status != MergeStatus::NONE) {
            const auto children = getChildren(expr);
            if constexpr (MergeIdentity<Expr>::status == MergeStatus::KEEP_LHS) {
                return std::get<0>(children);
            } else {
                return std::get<1>(children);
            }
        } else {
            return expr;
        }
    }*/

    template<Expression Expr>
    constexpr auto simplifyNodeCompileTime(Expr expr) {
        return expr;
    }

    /*
     * ADD
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Add<Lhs, Rhs> add) requires(Lhs::resolve() == 0) {
        return std::get<1>(getChildren(add));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Add<Lhs, Rhs> add) requires(Rhs::resolve() == 0 and util::notZero<Lhs>) {
        return std::get<0>(getChildren(add));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Add<Lhs, Rhs> /*add*/) requires(
            util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs> and
            (util::notZero<Lhs> and util::notZero<Rhs>)) {
        constexpr auto val = Lhs::resolve() + Rhs::resolve();
        return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
    }

    /*
     * SUB
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Sub<Lhs, Rhs> sub) requires(Rhs::resolve() == 0) {
        return std::get<0>(getChildren(sub));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Sub<Lhs, Rhs> /*sub*/) requires(
            util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs> and util::notZero<Lhs>) {
        constexpr auto val = Lhs::resolve() - Rhs::resolve();
        return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
    }

    /*
     * MUL
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> /*mul*/) requires(Lhs::resolve() == 0) {
        return CompiletimeConstant<int, 0>{};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> /*mul*/) requires(Rhs::resolve() == 0 and util::notZero<Lhs>) {
        return CompiletimeConstant<int, 0>{};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> mul) requires(Lhs::resolve() == 1 and util::notZero<Rhs>) {
        return std::get<1>(getChildren(mul));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> mul) requires(Rhs::resolve() == 1 and util::notZero<Lhs>) {
        return std::get<0>(getChildren(mul));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> /*mul*/) requires(
            util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs> and
            (util::notZero<Lhs> and util::notZero<Rhs> and util::notOne<Lhs> and util::notOne<Rhs>)) {
        constexpr auto val = Lhs::resolve() - Rhs::resolve();
        return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
    }

    /*
     * DIV
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Div<Lhs, Rhs> /*div*/) requires(Lhs::resolve() == 0) {
        return CompiletimeConstant<int, 0>{};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Div<Lhs, Rhs> div) requires(Rhs::resolve() == 1 and util::notZero<Lhs>) {
        return std::get<0>(getChildren(div));
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Div<Lhs, Rhs> /*div*/) requires(
            util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs> and
            (util::notZero<Lhs> and util::notOne<Rhs>)) {
        constexpr auto val = Lhs::resolve() / Rhs::resolve();
        return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
    }

} // namespace sym::simplifier


#endif // SYM_COMPILETIME_HPP
