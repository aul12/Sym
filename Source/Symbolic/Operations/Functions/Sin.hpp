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
#include "../../Simplifier/GradientSimplifcation.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<Expression Expr>
    class Cos;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<Expression Expr>
    class Sin {
      public:
        explicit Sin(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const Sin<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Sin<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Sin<Expr_> &sin) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    Sin<Expr>::Sin(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Sin<Expr>::resolve(Bindings &&...bindings) const {
        return std::sin(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const Sin<Expr_> &x, const Variable<ID> &d) {
        auto cos = _GRADIENT_SIMPLIFY(Cos{x.expr});
        return _GRADIENT_SIMPLIFY(Mul{cos, gradient(x.expr, d)});
    }

    template<Expression Expr_>
    auto toString(const Sin<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const Sin<Expr_> &sin) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(sin.expr);
    }
} // namespace sym

#include "../../Simplifier/CompileTime.hpp"
#include "../Mul.hpp"
#include "Cos.hpp"

#endif // SYM_SIN_HPP
