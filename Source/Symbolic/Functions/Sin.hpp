/**
 * @file Sin.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_SIN_HPP
#define GRADIENTOPTIMIZATION_SIN_HPP

#include <cmath>

#include "../Expression.hpp"
#include "../Mul.hpp"
#include "../Variable.hpp"
#include "Cos.hpp"

namespace sym {
    template<Expression Expr>
    class Cos;

    template<Expression Expr>
    class Sin {
      public:
        using type = typename Expr::type;

        explicit Sin(Expr expr);

        auto resolve() const -> type;

        template<Expression Expr_>
        friend auto gradient(const Sin<Expr_> &x, const Variable<typename Expr_::type> &d);

        template<Expression Expr_>
        friend auto toString(const Sin<Expr_> &x) -> std::string;

        static constexpr auto isConstant() -> bool;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Sin<Expr>::Sin(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    auto Sin<Expr>::resolve() const -> type {
        return std::sin(expr.resolve());
    }

    template<Expression Expr_>
    auto gradient(const Sin<Expr_> &x, const Variable<typename Expr_::type> &d) {
        return Mul{Cos{x.expr}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Sin<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }

    template<Expression Expr>
    constexpr auto Sin<Expr>::isConstant() -> bool {
        return Expr::isConstant();
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_SIN_HPP
