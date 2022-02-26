/**
 * @file ArcSin.hpp
 * @author paul
 * @date 09.12.21
 * Description here TODO
 */
#ifndef SYM_ARCSIN_HPP
#define SYM_ARCSIN_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Simplifier/GradientSimplifcation.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Div;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<Expression Lhs, Expression Rhs>
    class Sub;

    template<sym::Expression Expr>
    class Sqrt;

    template<sym::Expression Expr>
    class ArcSin {
      public:
        explicit constexpr ArcSin(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const ArcSin<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const ArcSin<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const ArcSin<Expr_> &sin) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<sym::Expression Expr>
    constexpr ArcSin<Expr>::ArcSin(Expr expr) : expr{expr} {
    }

    template<sym::Expression Expr>
    template<typename... Bindings>
    constexpr auto ArcSin<Expr>::resolve(Bindings &&...bindings) const {
        return std::asin(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const ArcSin<Expr_> &x, const Variable<ID> &d) {
        auto xSqr = _GRADIENT_SIMPLIFY(Mul{x.expr, x.expr});
        auto sqrtArg = _GRADIENT_SIMPLIFY(Sub{CompiletimeConstant<int, 1>{}, xSqr});
        auto sqrt = _GRADIENT_SIMPLIFY(Sqrt{sqrtArg});
        return _GRADIENT_SIMPLIFY(Div{gradient(x.expr, d), sqrt});
    }

    template<Expression Expr_>
    auto toString(const ArcSin<Expr_> &x) -> std::string {
        return "asin(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const ArcSin<Expr_> &sin) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(sin.expr);
    }
} // namespace sym

#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Sqrt.hpp"
#include "../../Simplifier/CompileTime.hpp"

#endif // SYM_ARCSIN_HPP
