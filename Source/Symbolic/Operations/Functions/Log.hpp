/**
 * @file Log.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_LOG_HPP
#define SYM_LOG_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<sym::Expression Lhs, sym::Expression Rhs>
    class Div;

    template<Expression Expr>
    class Log {
      public:
        explicit Log(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const Log<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Log<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Log<Expr_> &log) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    Log<Expr>::Log(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Log<Expr>::resolve(Bindings &&...bindings) const {
        return std::log(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const Log<Expr_> &x, const Variable<ID> &d) {
        return Div{gradient(x.expr, d), x.expr};
    }

    template<Expression Expr_>
    auto toString(const Log<Expr_> &x) -> std::string {
        return "log(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const Log<Expr_> &log) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(log.expr);
    }
} // namespace sym

#include "../Div.hpp"

#endif // SYM_LOG_HPP
