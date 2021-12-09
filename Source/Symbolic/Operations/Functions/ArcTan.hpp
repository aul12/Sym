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
#include "../../Variable.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Sqrt.hpp"

namespace sym {
    template<sym::Expression Expr>
    class ArcTan {
      public:
        explicit constexpr ArcTan(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const ArcTan<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const ArcTan<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const ArcTan<Expr_> &tan) -> std::tuple<Expr_>;

      private:
        Expr expr;
    };

    template<sym::Expression Expr>
    constexpr ArcTan<Expr>::ArcTan(Expr expr) : expr{expr} {
    }

    template<sym::Expression Expr>
    template<typename... Bindings>
    constexpr auto ArcTan<Expr>::resolve(Bindings &&...bindings) const {
        return std::atan(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const ArcTan<Expr_> &x, const Variable<ID> &d) {
        return Div{gradient(x.expr, d), Add{CompiletimeConstant<int, 1>{}, Mul{x.expr, x.expr}}};
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

#endif // SYM_ARCTAN_HPP
