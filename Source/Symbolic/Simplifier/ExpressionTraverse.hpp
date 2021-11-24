/**
 * @file ExpressionTraverse.hpp
 * @author paul
 * @date 24.11.21
 * Description here TODO
 */
#ifndef SYM_EXPRESSIONTRAVERSE_HPP
#define SYM_EXPRESSIONTRAVERSE_HPP

#include "../Expression.hpp"

namespace sym::simplifier {
    template<std::size_t index, sym::Expression... Children>
    auto traverseChildren(const std::tuple<Children...> &children);

    template<sym::Expression Expr, typename FEnter, typename FLeave>
    requires(IsExpression<decltype(std::declval<FEnter>()(std::declval<Expr>()))>::val and IsExpression<
             decltype(std::declval<FLeave>()(std::declval<Expr>()))>::val) auto traverseExpression(Expr &&expr,
                                                                                                   FEnter &&fEnter,
                                                                                                   FLeave &&fLeave) {
        auto enterExpr = fEnter(expr);
        auto children = getChildren(enterExpr);
        traverseChildren<0>(children);
        return fLeave(expr);
    }

    template<std::size_t index, typename FEnter, typename FLeave, sym::Expression... Children>
    auto traverseChildren(std::tuple<Children...> &children, FEnter fEnter, FLeave fLeave) {
        std::get<index>(children) = traverseExpression(std::get<index>(children), fEnter, fLeave);
        if constexpr (index + 1 < sizeof...(Children)) {
            traverseChildren<index+1>(children, fEnter, fLeave);
        }
    }
} // namespace sym::simplifier

#endif // SYM_EXPRESSIONTRAVERSE_HPP
