/**
 * @file ArcCos.hpp
 * @author paul
 * @date 09.12.21
 * Description here TODO
 */
#ifndef SYM_ARCCOS_HPP
#define SYM_ARCCOS_HPP

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

    template<Expression Expr>
    class Sqrt;

    template<Expression Expr>
    class ArcCos {
      public:
        explicit constexpr ArcCos(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, fixed_string ID>
        constexpr friend auto gradient(const ArcCos<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const ArcCos<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const ArcCos<Expr_> &cos) -> std::tuple<Expr_>;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    constexpr ArcCos<Expr>::ArcCos(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto ArcCos<Expr>::resolve(Bindings &&...bindings) const {
        return std::acos(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, fixed_string ID>
    constexpr auto gradient(const ArcCos<Expr_> &x, const Variable<ID> &d) {
        auto nom = _GRADIENT_SIMPLIFY(Sub{CompiletimeConstant<int, 0>{}, gradient(x.expr, d)});
        auto denom_arg = _GRADIENT_SIMPLIFY(Sub{CompiletimeConstant<int, 1>{}, Mul{x.expr, x.expr}});
        auto denom = _GRADIENT_SIMPLIFY(Sqrt{denom_arg});
        return _GRADIENT_SIMPLIFY(Div{nom, denom});
    }

    template<Expression Expr_>
    auto toString(const ArcCos<Expr_> &x) -> std::string {
        return "acos(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const ArcCos<Expr_> &cos) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(cos.expr);
    }
} // namespace sym

#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Sqrt.hpp"
#include "../../Simplifier/CompileTime.hpp"

#endif // SYM_ARCCOS_HPP
