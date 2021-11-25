/**
 * @file Sqrt.hpp
 * @author paul
 * @date 16.11.21
 * Description here TODO
 */
#ifndef SYM_SQRT_HPP
#define SYM_SQRT_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"
#include "../Div.hpp"

namespace sym {
    template<Expression Expr>
    class Sqrt {
      public:
        explicit Sqrt(Expr expr) : expr{expr} {
        }

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const {
            return std::sqrt(expr.resolve(std::forward<Bindings>(bindings)...));
        }

        template<Expression Expr_, std::size_t ID>
        constexpr friend auto gradient(const Sqrt<Expr_> &sqrt, const Variable<ID> &id);

        template<Expression Expr_>
        friend auto toString(const Sqrt<Expr_> &x) -> std::string;

        template<Expression Expr_>
        constexpr friend auto getChildren(const Sqrt<Expr_> &sqrt) -> std::tuple<Expr_>;
      private:
        Expr expr;
    };

    template<Expression Expr_, std::size_t ID>
    constexpr auto gradient(const Sqrt<Expr_> &sqrt, const Variable<ID> &id) {
        /*
         * d/dx \sqrt(f(x)) = 0.5 * 1 / \sqrt{f(x)} * d/dx f(x)
         *  = d/dx f(x) / (2 * \sqrt{f(x)})
         */
        return Div{gradient(sqrt.expr, id), Mul{CompiletimeConstant<int, 2>{}, Sqrt{sqrt.expr}}};
    }

    template<Expression Expr_>
    auto toString(const Sqrt<Expr_> &x) -> std::string {
        return "sqrt(" + toString(x.expr) + ")";
    }

    template<Expression Expr_>
    constexpr auto getChildren(const Sqrt<Expr_> &sqrt) -> std::tuple<Expr_> {
        return std::tuple<Expr_>(sqrt.expr);
    }
} // namespace sym

#endif // SYM_SQRT_HPP
