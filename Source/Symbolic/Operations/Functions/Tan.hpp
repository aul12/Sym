/**
 * @file Tan.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_TAN_HPP
#define SYM_TAN_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Div;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<Expression Expr>
    class Cos;

    template<Expression Expr>
    class Tan {
      public:
        explicit Tan(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const Tan<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Tan<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Tan<Expr_> &tan) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    Tan<Expr>::Tan(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Tan<Expr>::resolve(Bindings &&...bindings) const {
        return std::tan(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const Tan<Expr_> &x, const Variable<ID> &d) {
        return Div{gradient(x.expr, d), Mul{Cos{x}, Cos{x}}};
    }

    template<Expression Expr_>
    auto toString(const Tan<Expr_> &x) -> std::string {
        return "tan(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const Tan<Expr_> &tan) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(tan.expr);
    }
} // namespace sym

#include "../Div.hpp"
#include "../Mul.hpp"
#include "Cos.hpp"

#endif // SYM_TAN_HPP
