/**
 * @file Cos.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_COS_HPP
#define GRADIENTOPTIMIZATION_COS_HPP

#include <Symbolic/Mul.hpp>
#include <cmath>

#include "../Constant.hpp"
#include "../Expression.hpp"
#include "../Variable.hpp"
#include "Sin.hpp"

namespace sym {
    template<Expression Expr>
    class Sin;

    template<Expression Expr>
    class Cos {
      public:
        using type = typename Expr::type;

        explicit Cos(Expr expr);

        auto resolve() const -> type;

        template<Expression Expr_>
        friend auto gradient(const Cos<Expr_> &x, const Variable<typename Expr_::type> &d);

        template<Expression Expr_>
        friend auto toString(const Cos<Expr_> &x) -> std::string;

        static constexpr auto isConstant() -> bool;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Cos<Expr>::Cos(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    auto Cos<Expr>::resolve() const -> type {
        return std::cos(expr.resolve());
    }

    template<Expression Expr_>
    auto gradient(const Cos<Expr_> &x, const Variable<typename Expr_::type> &d) {
        return Mul{Mul{Constant<typename Expr_::type>{-1}, Sin{x.expr}}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Cos<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }

    template<Expression Expr>
    constexpr auto Cos<Expr>::isConstant() -> bool {
        return Expr::isConstant();
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_COS_HPP
