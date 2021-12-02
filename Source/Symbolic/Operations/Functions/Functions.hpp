/**
 * @file Functions.hpp.h
 * @author paul
 * @date 15.01.21
 * Description here TODO
 */
#ifndef SYM_FUNCTIONS_HPP
#define SYM_FUNCTIONS_HPP

#include "Cos.hpp"
#include "Exp.hpp"
#include "Sin.hpp"
#include "Sqrt.hpp"
#include "Tan.hpp"
#include "../../Operators.hpp"
#include "../Ternary.hpp"

namespace std { // NOLINT everything is fine specialization for custom types is allowed.
    template<sym::Expression Expr>
    auto sin(Expr &&expr) {
        return sym::Sin<Expr>{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto cos(Expr &&expr) {
        return sym::Cos<Expr>{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto tan(Expr &&expr) {
        return sym::Tan<Expr>{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto sqrt(Expr &&expr) {
        return sym::Sqrt<Expr>{std::forward<Expr>(expr)};
    }

    template<sym::Expression Expr>
    auto exp(Expr &&expr) {
        return sym::Exp<Expr>{std::forward<Expr>(expr)};
    }

    template<sym::Expression Lhs, sym::Expression Rhs>
    auto max(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs > rhs, lhs, rhs};
    }

    template<sym::Expression Lhs, sym::Expression Rhs>
    auto min(const Lhs &lhs, const Rhs &rhs) {
        return sym::Ternary{lhs < rhs, lhs, rhs};
    }

    template<sym::Expression Expr>
    auto abs(const Expr &expr) {
        return std::max(expr, -expr);
    }
} // namespace std

#endif // SYM_FUNCTIONS_HPP
