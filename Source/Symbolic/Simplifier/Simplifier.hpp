/**
 * @file Simplifier.hpp
 * @author paul
 * @date 24.11.21
 * Description here TODO
 */
#ifndef SYM_SIMPLIFIER_HPP
#define SYM_SIMPLIFIER_HPP

#include "CompileTime.hpp"
#include "Runtime.hpp"
#include "ExpressionTraverse.hpp"

namespace sym::simplifier {
    template<Expression Expr>
    auto simplifyWhileChangeImpl(Expr expr) {
        auto simplified = simplifyNodeCompileTime(expr);
        //if constexpr (std::is_same_v<decltype(simplified), std::remove_cvref_t<decltype(expr)>>) {
            return simplifyRuntime(simplified);
        /*} else {
            return simplifyWhileChangeImpl(simplified);
        }*/
    }

    template<Expression Expr>
    auto simplify(Expr expr) {
        auto simplifyWhileChange = [](auto &&expr) { return simplifyWhileChangeImpl(expr); };
        return traverseExpression(expr, simplifyWhileChange, simplifyWhileChange);
    }
} // namespace sym::simplifier

#endif // SYM_SIMPLIFIER_HPP
