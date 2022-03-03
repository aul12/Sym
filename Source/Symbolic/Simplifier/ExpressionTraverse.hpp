/**
 * @file ExpressionTraverse.hpp
 * @author paul
 * @date 24.11.21
 * Description here TODO
 */
#ifndef SYM_EXPRESSIONTRAVERSE_HPP
#define SYM_EXPRESSIONTRAVERSE_HPP

#include "../Expression.hpp"
#include "../Vector.hpp"

namespace sym::simplifier {
    template<template<typename...> typename T, typename... OldArgs>
    struct ConstructWithNewTemplateArgs {
        template<typename... NewArgs>
        static auto construct(NewArgs... newArgs) {
            return T<NewArgs...>(std::forward<NewArgs>(newArgs)...);
        }
    };

    template<template<typename...> typename T, typename... OldArgs>
    auto getConstructWithNewTemplateArgs(T<OldArgs...>) {
        return ConstructWithNewTemplateArgs<T, OldArgs...>{};
    }

    template<Expression Expr, typename FEnter, typename FLeave>
    requires(IsExpression<decltype(std::declval<FEnter>()(std::declval<Expr>()))>::val and IsExpression<
             decltype(std::declval<FLeave>()(std::declval<Expr>()))>::val) auto traverseExpression(Expr &&expr,
                                                                                                   FEnter &&fEnter,
                                                                                                   FLeave &&fLeave) {
        auto enterExpr = fEnter(expr);
        auto children = getChildren(enterExpr);
        if constexpr (std::tuple_size_v < decltype(children) >> 0) {
            auto visitedChildren = mapTuple(children, [fEnter, fLeave](Expression auto child) {
                return traverseExpression(child, fEnter, fLeave);
            });
            auto visitedNode = std::apply(
                    [](auto... args) {
                        using Factory = decltype(getConstructWithNewTemplateArgs(enterExpr));
                        return Factory::template construct<std::remove_cvref_t<decltype(args)>...>(
                                std::forward<decltype(args)>(args)...);
                    },
                    visitedChildren);
            return fLeave(visitedNode);
        } else {
            return fLeave(enterExpr);
        }
    }
} // namespace sym::simplifier

#endif // SYM_EXPRESSIONTRAVERSE_HPP
