/**
 * @file Cos.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_COS_HPP
#define SYM_COS_HPP

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
        explicit Cos(Expr expr);

        template<typename... Bindings>
        auto resolve(Bindings... bindings) const;

        template<Expression Expr_, std::size_t ID>
        friend auto gradient(const Cos<Expr_> &x, const Variable<ID> &d);

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
    auto Cos<Expr>::resolve(Bindings... bindings) const {
        return std::cos(expr.resolve(bindings...));
    }

    template<Expression Expr_, std::size_t ID>
    auto gradient(const Cos<Expr_> &x, const Variable<ID> &d) {
        return Mul{Mul{Constant{-1}, Sin{x.expr}}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Cos<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }
} // namespace sym

#endif // SYM_COS_HPP
