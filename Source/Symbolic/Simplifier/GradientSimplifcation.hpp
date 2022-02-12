/**
 * @file GradientSimplifcation.hpp
 * @author paul
 * @date 12.02.22
 * Description here TODO
 */
#ifndef SYM_GRADIENTSIMPLIFCATION_HPP
#define SYM_GRADIENTSIMPLIFCATION_HPP

#include "../Expression.hpp"

#ifdef _GRADIENT_SIMPLIFICATION
    #define _GRADIENT_SIMPLIFY(...) sym::simplifier::simplifyNodeCompileTime(__VA_ARGS__)
#else
    #define _GRADIENT_SIMPLIFY(...) __VA_ARGS__
#endif

namespace sym::simplifier {
    template<Expression Expr>
    constexpr auto simplifyNodeCompileTime(Expr expr);
} // namespace sym::simplifier

#endif // SYM_GRADIENTSIMPLIFCATION_HPP
