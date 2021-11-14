/**
 * @file Operators.hpp
 * @author paul
 * @date 05.09.20
 * Description here TODO
 */
#ifndef SYM_OPERATORS_HPP
#define SYM_OPERATORS_HPP

#include "Operations/Add.hpp"
#include "Operations/Div.hpp"
#include "Operations/Mul.hpp"
#include "Operations/Sub.hpp"

namespace sym {
    // Expr op Expr
    template<Expression Lhs, Expression Rhs>
    constexpr auto operator+(Lhs lhs, Rhs rhs) -> Add<Lhs, Rhs> {
        return Add<Lhs, Rhs>{lhs, rhs};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto operator*(Lhs lhs, Rhs rhs) -> Mul<Lhs, Rhs> {
        return Mul<Lhs, Rhs>{lhs, rhs};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto operator/(Lhs lhs, Rhs rhs) -> Div<Lhs, Rhs> {
        return Div<Lhs, Rhs>{lhs, rhs};
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto operator-(Lhs lhs, Rhs rhs) -> Sub<Lhs, Rhs> {
        return Sub<Lhs, Rhs>{lhs, rhs};
    }

    // T op Expr
    template<typename Lhs, Expression Rhs>
    constexpr auto operator+(Lhs lhs, Rhs rhs) -> Add<RuntimeConstant<Lhs>, Rhs> {
        return Add<RuntimeConstant<Lhs>, Rhs>{RuntimeConstant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator*(Lhs lhs, Rhs rhs) -> Mul<RuntimeConstant<Lhs>, Rhs> {
        return Mul<RuntimeConstant<Lhs>, Rhs>{RuntimeConstant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator/(Lhs lhs, Rhs rhs) -> Div<RuntimeConstant<Lhs>, Rhs> {
        return Div<RuntimeConstant<Lhs>, Rhs>{RuntimeConstant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator-(Lhs lhs, Rhs rhs) -> Sub<RuntimeConstant<Lhs>, Rhs> {
        return Sub<RuntimeConstant<Lhs>, Rhs>{RuntimeConstant<Lhs>{lhs}, rhs};
    }

    // Expr op T
    template<Expression Lhs, typename Rhs>
    constexpr auto operator+(Lhs lhs, Rhs rhs) -> Add<Lhs, RuntimeConstant<Rhs>> {
        return Add<Lhs, RuntimeConstant<Rhs>>{lhs, RuntimeConstant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator*(Lhs lhs, Rhs rhs) -> Mul<Lhs, RuntimeConstant<Rhs>> {
        return Mul<Lhs, RuntimeConstant<Rhs>>{lhs, RuntimeConstant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator/(Lhs lhs, Rhs rhs) -> Div<Lhs, RuntimeConstant<Rhs>> {
        return Div<Lhs, RuntimeConstant<Rhs>>{lhs, RuntimeConstant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator-(Lhs lhs, Rhs rhs) -> Sub<Lhs, RuntimeConstant<Rhs>> {
        return Sub<Lhs, RuntimeConstant<Rhs>>{lhs, RuntimeConstant<Rhs>{rhs}};
    }
} // namespace sym

#endif // SYM_OPERATORS_HPP
