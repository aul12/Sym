/**
 * @file Util.hpp
 * @author paul
 * @date 12.02.22
 * Description here TODO
 */
#ifndef SYM_UTIL_HPP
#define SYM_UTIL_HPP

#include "../CompiletimeConstant.hpp"
#include "../Expression.hpp"
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

    template<typename T, typename Val, Val val_>
    struct CompiletimeConstantAndEqual {
        static constexpr auto val = false;
    };

    template<typename T, T t, typename Val, Val val_>
    struct CompiletimeConstantAndEqual<CompiletimeConstant<Val, val_>, T, t> {
        static constexpr auto val = t == val_;
    };

    template<Expression Expr>
    constexpr auto notZero = not(CompiletimeConstantAndEqual<Expr, int, 0>::val);

    template<Expression Expr>
    constexpr auto notOne = not(CompiletimeConstantAndEqual<Expr, int, 1>::val);


} // namespace sym::simplifier::util

#endif // SYM_UTIL_HPP
