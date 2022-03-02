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
#include "../Operations/Ternary.hpp"
#include "Util.hpp"


namespace sym::simplifier {
    /*
     * No Simplification possible
     */
    template<Expression Expr>
    constexpr auto simplifyNodeCompileTime(Expr expr) {
        return expr;
    }

    /*
     * ADD
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Add<Lhs, Rhs> add) {
        if constexpr (util::isZero<Lhs>) {
            return std::get<1>(getChildren(add));
        } else if constexpr (util::isZero<Rhs>) {
            return std::get<0>(getChildren(add));
        } else if constexpr (util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs>) {
            constexpr auto val = Lhs::resolve() + Rhs::resolve();
            return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
        } else {
            return add;
        }
    }

    /*
     * SUB
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Sub<Lhs, Rhs> sub) {
        if constexpr (util::isZero<Rhs>) {
            return std::get<0>(getChildren(sub));
        } else if constexpr (util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs>) {
            constexpr auto val = Lhs::resolve() - Rhs::resolve();
            return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
        } else {
            return sub;
        }
    }

    /*
     * MUL
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Mul<Lhs, Rhs> mul) {
        if constexpr (util::isZero<Lhs> or util::isZero<Rhs>) {
            return CompiletimeConstant<int, 0>{};
        } else if constexpr (util::isOne<Lhs>) {
            return std::get<1>(getChildren(mul));
        } else if constexpr (util::isOne<Rhs>) {
            return std::get<0>(getChildren(mul));
        } else if constexpr (util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs>) {
            constexpr auto val = Lhs::resolve() * Rhs::resolve();
            return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
        } else {
            return mul;
        }
    }

    /*
     * DIV
     */
    template<Expression Lhs, Expression Rhs>
    constexpr auto simplifyNodeCompileTime(Div<Lhs, Rhs> div) {
        if constexpr (util::isZero<Lhs>) {
            return CompiletimeConstant<int, 0>{};
        } else if constexpr (util::isOne<Rhs>) {
            return std::get<0>(getChildren(div));
        } else if constexpr (util::isCompileTimeConstant<Lhs> and util::isCompileTimeConstant<Rhs>) {
            constexpr auto val = Lhs::resolve() / Rhs::resolve();
            return CompiletimeConstant<std::remove_const_t<decltype(val)>, val>{};
        } else {
            return div;
        }
    }

    /*
     * TERNARY
     */
    template<Expression Cond, Expression TrueVal, Expression FalseVal>
    constexpr auto simplifyNodeCompileTime(Ternary<Cond, TrueVal, FalseVal> ternary) {
        if constexpr (util::isCompileTimeConstant<Cond>) {
            if constexpr (Cond::resolve()) {
                return std::get<1>(getChildren(ternary));
            } else {
                return std::get<2>(getChildren(ternary));
            }
        } else if constexpr (util::isCompileTimeConstant<TrueVal> and util::isCompileTimeConstant<FalseVal>) {
            if constexpr (TrueVal::resolve() == FalseVal::resolve()) {
                return std::get<1>(getChildren(ternary));
            } else {
                return ternary;
            }
        } else {
            return ternary;
        }
    }

} // namespace sym::simplifier


#endif // SYM_COMPILETIME_HPP
