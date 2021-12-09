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
#include "../../Variable.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Sqrt.hpp"

namespace sym {
    template<sym::Expression Expr>
    class ArcCos {
      public:
        explicit constexpr ArcCos(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const ArcCos<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const ArcCos<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const ArcCos<Expr_> &cos) -> std::tuple<Expr_>;

      private:
        Expr expr;
    };

    template<sym::Expression Expr>
    constexpr ArcCos<Expr>::ArcCos(Expr expr) : expr{expr} {
    }

    template<sym::Expression Expr>
    template<typename... Bindings>
    constexpr auto ArcCos<Expr>::resolve(Bindings &&...bindings) const {
        return std::acos(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const ArcCos<Expr_> &x, const Variable<ID> &d) {
        return Div{Sub{CompiletimeConstant<int, 0>{}, gradient(x.expr, d)},
                   Sqrt{Sub{CompiletimeConstant<int, 1>{}, Mul{x.expr, x.expr}}}};
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

#endif // SYM_ARCCOS_HPP
