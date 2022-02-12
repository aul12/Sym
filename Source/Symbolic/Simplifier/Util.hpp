/**
 * @file Util.hpp
 * @author paul
 * @date 12.02.22
 * Description here TODO
 */
#ifndef SYM_UTIL_HPP
#define SYM_UTIL_HPP

#include "../Expression.hpp"
#include "../CompiletimeConstant.hpp"
#include "../RuntimeConstant.hpp"

namespace sym::simplifier::util {
    template<Expression Expr>
    struct IsCompileTimeConstant {
        static constexpr auto val = false;
    };

    template<typename T, T val_>
    struct IsCompileTimeConstant<CompiletimeConstant<T, val_>> {
        static constexpr auto val = true;
    };

    template<Expression Expr>
    constexpr auto isCompileTimeConstant = IsCompileTimeConstant<Expr>::val;


    template<Expression Expr>
    struct IsRuntimeConstant {
        static constexpr auto val = false;
    };

    template<typename T>
    struct IsRuntimeConstant<RuntimeConstant<T>> {
        static constexpr auto val = true;
    };

    template<Expression Expr>
    constexpr auto isRuntimeConstant = IsRuntimeConstant<Expr>::val;

    template<Expression Expr>
    constexpr auto isConstant = isRuntimeConstant<Expr> or isCompileTimeConstant<Expr>;

} // namespace sym::simplifier::util

#endif // SYM_UTIL_HPP
