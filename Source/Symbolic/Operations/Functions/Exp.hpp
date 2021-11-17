/**
 * @file Exp.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_EXP_HPP
#define SYM_EXP_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"
#include "../Mul.hpp"

namespace sym {
    template<Expression Expr>
    class Exp {
      public:
        explicit Exp(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const Exp<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Exp<Expr_> &x) -> std::string;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Exp<Expr>::Exp(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Exp<Expr>::resolve(Bindings &&...bindings) const {
        return std::exp(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const Exp<Expr_> &x, const Variable<ID> &d) {
        return Mul{Exp{x.expr}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Exp<Expr_> &x) -> std::string {
        return "exp(" + toString(x.expr) + ")";
    }
} // namespace sym

#endif // SYM_EXP_HPP
