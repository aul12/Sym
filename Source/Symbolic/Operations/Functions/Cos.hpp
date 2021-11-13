/**
 * @file Cos.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_COS_HPP
#define GRADIENTOPTIMIZATION_COS_HPP

#include <cmath>

#include "../../Constant.hpp"
#include "../../Expression.hpp"
#include "../../Variable.hpp"
#include "Sin.hpp"
#include "Symbolic/Operations//Mul.hpp"

namespace sym {
    template<Expression Expr>
    class Sin;

    template<Expression Expr>
    class Cos {
      public:
        using type = typename Expr::type;

        explicit Cos(Expr expr);

        template<typename... Bindings>
        auto resolve(Bindings... bindings) const -> type;

        template<Expression Expr_, std::size_t ID>
        friend auto gradient(const Cos<Expr_> &x, const Variable<typename Expr_::type, ID> &d);

        template<Expression Expr_>
        friend auto toString(const Cos<Expr_> &x) -> std::string;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Cos<Expr>::Cos(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    auto Cos<Expr>::resolve(Bindings... bindings) const -> type {
        return std::cos(expr.resolve(bindings...));
    }

    template<Expression Expr_, std::size_t ID>
    auto gradient(const Cos<Expr_> &x, const Variable<typename Expr_::type, ID> &d) {
        return Mul{Mul{Constant<typename Expr_::type>{-1}, Sin{x.expr}}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Cos<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_COS_HPP
