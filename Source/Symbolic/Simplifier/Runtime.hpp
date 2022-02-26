/**
 * @file Runtime.hpp
 * @author paul
 * @date 25.11.21
 * Description here TODO
 */
#ifndef SYM_RUNTIME_HPP
#define SYM_RUNTIME_HPP

#include "../Expression.hpp"
#include "../Vector.hpp"
#include "Util.hpp"

namespace sym::simplifier {
    namespace impl {
        template<typename T>
        struct AllConstant {};

        template<typename... Ts>
        struct AllConstant<std::tuple<Ts...>> {
            static constexpr auto val = (... and (util::isRuntimeConstant<Ts> or util::isCompileTimeConstant<Ts>));
        };
    } // namespace impl


    template<Expression Expr>
    constexpr auto simplifyRuntime(Expr expr) {
        using Children = decltype(getChildren(expr));
        if constexpr (std::tuple_size_v < Children >> 0) {
            constexpr auto allConstant = impl::AllConstant<Children>::val;

            if constexpr (allConstant) {
                return RuntimeConstant{expr.resolve()};
            } else {
                return expr;
            }
        } else {
            return expr;
        }
    }

    template<Expression... Exprs>
    constexpr auto simplifyRuntime(const Vector<Exprs...> &vec) {
        return vec;
    }
} // namespace sym::simplifier

#endif // SYM_RUNTIME_HPP
