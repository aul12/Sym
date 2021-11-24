/**
 * @file Cos.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_COS_HPP
#define SYM_COS_HPP

#include <cmath>

#include "../../CompiletimeConstant.hpp"
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
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const Cos<Expr_> &x, const Variable<ID> &d);

        template<Expression Expr_>
        friend auto toString(const Cos<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Cos<Expr_> &cos) -> std::tuple<Expr_>;

      private:
        Expr expr;
    };

    template<Expression Expr>
    Cos<Expr>::Cos(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Cos<Expr>::resolve(Bindings &&...bindings) const {
        return std::cos(expr.resolve(std::forward<Bindings>(bindings)...));
    }

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const Cos<Expr_> &x, const Variable<ID> &d) {
        return Mul{Mul{CompiletimeConstant<int, -1>{}, Sin{x.expr}}, gradient(x.expr, d)};
    }

    template<Expression Expr_>
    auto toString(const Cos<Expr_> &x) -> std::string {
        return "cos(" + toString(x.expr) + ")";
    }

    template<typename Expr_>
    auto getChildren(const Cos<Expr_> &cos) -> std::tuple<Expr_> {
        return std::make_tuple(cos.expr);
    }
} // namespace sym

#endif // SYM_COS_HPP
