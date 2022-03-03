/**
 * @file ArcTan.hpp
 * @author paul
 * @date 09.12.21
 * Description here TODO
 */
#ifndef SYM_ARCTAN_HPP
#define SYM_ARCTAN_HPP

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
    class Add;

    template<Expression Expr>
    class ArcTan {
      public:
        explicit constexpr ArcTan(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const ArcTan<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const ArcTan<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const ArcTan<Expr_> &tan) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    constexpr ArcTan<Expr>::ArcTan(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto ArcTan<Expr>::resolve(Bindings &&...bindings) const {
        return std::atan(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const ArcTan<Expr_> &x, const Variable<ID> &d) {
        auto xSqr = _GRADIENT_SIMPLIFY(Mul{x.expr, x.expr});
        auto denom = _GRADIENT_SIMPLIFY(Add{CompiletimeConstant<int, 1>{}, xSqr});
        return _GRADIENT_SIMPLIFY(Div{gradient(x.expr, d), denom});
    }

    template<Expression Expr_>
    auto toString(const ArcTan<Expr_> &x) -> std::string {
        return "atan(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const ArcTan<Expr_> &tan) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(tan.expr);
    }
} // namespace sym

#include "../Add.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../../Simplifier/CompileTime.hpp"

#endif // SYM_ARCTAN_HPP
