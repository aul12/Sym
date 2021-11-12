/**
 * @file Functions.hpp.h
 * @author paul
 * @date 15.01.21
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_FUNCTIONS_HPP
#define GRADIENTOPTIMIZATION_FUNCTIONS_HPP

#include "Cos.hpp"
#include "Sin.hpp"
#include "Tan.hpp"

namespace std { // NOLINT everything is fine specialization for custom types is allowed.
    template<grad::sym::Expression Expr>
    auto sin(Expr expr) {
        return grad::sym::Sin<Expr>{expr};
    }

    template<grad::sym::Expression Expr>
    auto cos(Expr expr) {
        return grad::sym::Cos<Expr>{expr};
    }

    template<grad::sym::Expression Expr>
    auto tan(Expr expr) {
        return grad::sym::Tan<Expr>{expr};
    }
} // namespace std

#endif // GRADIENTOPTIMIZATION_FUNCTIONS_HPP
