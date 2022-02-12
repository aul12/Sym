/**
 * @file Functions.hpp.h
 * @author paul
 * @date 15.01.21
 * Description here TODO
 */
#ifndef SYM_FUNCTIONS_HPP
#define SYM_FUNCTIONS_HPP

#include "../../Operators.hpp"
#include "../Ternary.hpp"
#include "ArcCos.hpp"
#include "ArcSin.hpp"
#include "ArcTan.hpp"
#include "Cos.hpp"
#include "Exp.hpp"
#include "Log.hpp"
#include "Pow.hpp"
#include "Sin.hpp"
#include "Sqrt.hpp"
#include "Tan.hpp"

namespace std { // NOLINT everything is fine specialization for custom types is allowed.
    template<sym::Expression Expr>
    auto sin(Expr &&expr) {
        return sym::Sin{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto cos(Expr &&expr) {
        return sym::Cos{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto tan(Expr &&expr) {
        return sym::Tan{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto sqrt(Expr &&expr) {
        return sym::Sqrt{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto exp(Expr &&expr) {
        return sym::Exp{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto asin(Expr &&expr) {
        return sym::ArcSin{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto acos(Expr &&expr) {
        return sym::ArcCos{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto atan(Expr &&expr) {
        return sym::ArcTan{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto log(Expr &&expr) {
        return sym::Log{std::forward<Expr>(expr)};
    }

    template<sym::Expression Base, sym::Expression Exp>
    auto pow(Base &&base, Exp &&exp) {
        return sym::Pow{std::forward<Base>(base), std::forward<Exp>(exp)};
    }

    template<typename Base, sym::Expression Exp>
    auto pow(Base &&base, Exp &&exp) {
        return sym::Pow{sym::RuntimeConstant{std::forward<Base>(base)}, std::forward<Exp>(exp)};
    }

    template<sym::Expression Base, typename Exp>
    auto pow(Base &&base, Exp &&exp) {
        return sym::Pow{std::forward<Base>(base), sym::RuntimeConstant{std::forward<Exp>(exp)}};
    }

    // Ternary based
    template<sym::Expression Lhs, sym::Expression Rhs>
    auto max(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs > rhs, lhs, rhs};
    }

    template<sym::Expression Lhs, sym::Expression Rhs>
    auto min(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs < rhs, lhs, rhs};
    }

    template<sym::Expression Lhs, typename Rhs>
    auto max(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs > sym::RuntimeConstant{rhs}, lhs, sym::RuntimeConstant{rhs}};
    }

    template<sym::Expression Lhs, typename Rhs>
    auto min(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs < sym::RuntimeConstant{rhs}, lhs, sym::RuntimeConstant{rhs}};
    }

    template<typename Lhs, sym::Expression Rhs>
    auto max(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{sym::RuntimeConstant{lhs} > rhs, sym::RuntimeConstant{lhs}, rhs};
    }

    template<typename Lhs, sym::Expression Rhs>
    auto min(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{sym::RuntimeConstant{lhs} < rhs, sym::RuntimeConstant{lhs}, rhs};
    }

    template<sym::Expression Expr>
    auto abs(const Expr &expr) {
        return sym::Ternary{expr > sym::CompiletimeConstant<int, 0>{}, expr, -expr};
    }
} // namespace std

#endif // SYM_FUNCTIONS_HPP
