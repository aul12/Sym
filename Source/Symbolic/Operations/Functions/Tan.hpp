/**
 * @file Tan.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_TAN_HPP
#define GRADIENTOPTIMIZATION_TAN_HPP

#include "../../Expression.hpp"
#include "../Div.hpp"
#include "Sin.hpp"

namespace sym {
    template<Expression Expr>
    class Tan {
      public:
        using type = typename Expr::type;

        explicit Tan(Expr expr);

        template<typename... Bindings>
        auto resolve(Bindings... bindings) const -> type;

        template<Expression Expr_, std::size_t ID>
        friend auto gradient(const Tan<Expr_> &x, const Variable<typename Expr_::type, ID> &d);

        template<Expression Expr_>
        friend auto toString(const Tan<Expr_> &x) -> std::string;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Tan<Expr>::Tan(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    auto Tan<Expr>::resolve(Bindings... bindings) const -> type {
        return std::tan(expr.resolve(bindings...));
    }

    template<Expression Expr_, std::size_t ID>
    auto gradient(const Tan<Expr_> &x, const Variable<typename Expr_::type, ID> &d) {
        return Div{gradient(x.expr, d), Mul{Cos{x}, Cos{x}}};
    }

    template<typename Expr_>
    auto toString(const Tan<Expr_> &x) -> std::string {
        return "tan(" + toString(x.expr) + ")";
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_TAN_HPP
