/**
 * @file Simplifier.hpp
 * @author paul
 * @date 24.11.21
 * Description here TODO
 */
#ifndef SYM_SIMPLIFIER_HPP
#define SYM_SIMPLIFIER_HPP

#include "CompileTime.hpp"
#include "ExpressionTraverse.hpp"
#include "Runtime.hpp"

namespace sym::simplifier {
    template<Expression Expr>
    auto simplify(Expr expr) {
        auto simplifyWhileChange = [](auto &&expr) {
            auto simplified = simplifyNodeCompileTime(expr);
            return simplifyRuntime(simplified);
        };
        return traverseExpression(expr, simplifyWhileChange, simplifyWhileChange);
    }
} // namespace sym::simplifier

#endif // SYM_SIMPLIFIER_HPP
