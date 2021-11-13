/**
 * @file Operators.hpp
 * @author paul
 * @date 05.09.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_OPERATORS_HPP
#define GRADIENTOPTIMIZATION_OPERATORS_HPP

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

    // Expr op T
    template<typename Lhs, Expression Rhs>
    constexpr auto operator+(Lhs lhs, Rhs rhs) -> Add<Constant<Lhs>, Rhs> {
        return Add<Constant<Lhs>, Rhs>{Constant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator*(Lhs lhs, Rhs rhs) -> Mul<Constant<Lhs>, Rhs> {
        return Mul<Constant<Lhs>, Rhs>{Constant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator/(Lhs lhs, Rhs rhs) -> Div<Constant<Lhs>, Rhs> {
        return Div<Constant<Lhs>, Rhs>{Constant<Lhs>{lhs}, rhs};
    }

    template<typename Lhs, Expression Rhs>
    constexpr auto operator-(Lhs lhs, Rhs rhs) -> Sub<Constant<Lhs>, Rhs> {
        return Sub<Constant<Lhs>, Rhs>{Constant<Lhs>{lhs}, rhs};
    }

    // T op Expr
    template<Expression Lhs, typename Rhs>
    constexpr auto operator+(Lhs lhs, Rhs rhs) -> Add<Lhs, Constant<Rhs>> {
        return Add<Lhs, Constant<Rhs>>{lhs, Constant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator*(Lhs lhs, Rhs rhs) -> Mul<Lhs, Constant<Rhs>> {
        return Mul<Lhs, Constant<Rhs>>{lhs, Constant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator/(Lhs lhs, Rhs rhs) -> Div<Lhs, Constant<Rhs>> {
        return Div<Lhs, Constant<Rhs>>{lhs, Constant<Rhs>{rhs}};
    }

    template<Expression Lhs, typename Rhs>
    constexpr auto operator-(Lhs lhs, Rhs rhs) -> Sub<Lhs, Constant<Rhs>> {
        return Sub<Lhs, Constant<Rhs>>{lhs, Constant<Rhs>{rhs}};
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_OPERATORS_HPP
