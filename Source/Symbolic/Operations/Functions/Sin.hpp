/**
 * @file Sin.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_SIN_HPP
#define SYM_SIN_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"
#include "../Mul.hpp"
#include "Cos.hpp"

namespace sym {
    template<Expression Expr>
    class Cos;

    template<Expression Expr>
    class Sin {
      public:
        explicit Sin(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const Sin<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Sin<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Sin<Expr_> &sin) -> std::tuple<Expr_>;
      private:
        Expr expr;
    };

    template<Expression Expr>
    Sin<Expr>::Sin(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Sin<Expr>::resolve(Bindings &&...bindings) const {
        return std::sin(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const Sin<Expr_> &x, const Variable<ID> &d) {
        return Mul{Cos{x.expr}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Sin<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const Sin<Expr_> &sin) -> std::tuple<Expr_> {
        return std::make_tuple(sin.expr);
    }
} // namespace sym

#endif // SYM_SIN_HPP
