/**
 * @file Functions.hpp.h
 * @author paul
 * @date 15.01.21
 * Description here TODO
 */
#ifndef SYM_FUNCTIONS_HPP
#define SYM_FUNCTIONS_HPP

#include "Cos.hpp"
#include "Sin.hpp"
#include "Tan.hpp"
#include "Sqrt.hpp"

namespace std { // NOLINT everything is fine specialization for custom types is allowed.
    template<sym::Expression Expr>
    auto sin(Expr expr) {
        return sym::Sin<Expr>{expr};
    }

    template<sym::Expression Expr>
    auto cos(Expr expr) {
        return sym::Cos<Expr>{expr};
    }

    template<sym::Expression Expr>
    auto tan(Expr expr) {
        return sym::Tan<Expr>{expr};
    }

    template<sym::Expression Expr>
    auto sqrt(Expr expr) {
        return sym::Sqrt<Expr>{expr};
    }
} // namespace std

#endif // SYM_FUNCTIONS_HPP
