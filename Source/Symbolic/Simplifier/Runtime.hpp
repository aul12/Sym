/**
 * @file Runtime.hpp
 * @author paul
 * @date 25.11.21
 * Description here TODO
 */
#ifndef SYM_RUNTIME_HPP
#define SYM_RUNTIME_HPP

#include "../Expression.hpp"
#include "CompileTime.hpp"

namespace sym::simplifier {
    namespace impl {
        template<Expression Expr>
        struct IsRuntimeConstant {
            static constexpr auto val = false;
        };

        template<typename T>
        struct IsRuntimeConstant<RuntimeConstant<T>> {
            static constexpr auto val = true;
        };

        template<Expression Expr>
        constexpr auto isRuntimeConstant = IsRuntimeConstant<Expr>::val;

        template<typename T>
        struct AllConstant {};

        template<typename... Ts>
        struct AllConstant<std::tuple<Ts...>> {
            static constexpr auto val = (... and (isRuntimeConstant<Ts> or isCompileTimeConstant<Ts>));
        };

        template<typename T>
        struct OneRuntimeConstant {};

        template<typename... Ts>
        struct OneRuntimeConstant<std::tuple<Ts...>> {
            static constexpr auto val = (... or isRuntimeConstant<Ts>);
        };

    } // namespace impl


    template<Expression Expr>
    constexpr auto simplifyRuntime(Expr expr) {
        using Children = decltype(getChildren(expr));
        constexpr auto allConstant = impl::AllConstant<Children>::val;
        constexpr auto atLeastOneRuntimeConstant = impl::OneRuntimeConstant<Children>::val;

        if constexpr (allConstant and atLeastOneRuntimeConstant) {
            return RuntimeConstant{expr.resolve()};
        } else {
            return expr;
        }
    }
} // namespace sym::simplifier

#endif // SYM_RUNTIME_HPP
