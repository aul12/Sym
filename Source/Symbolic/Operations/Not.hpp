/**
 * @file Not.hpp
 * @author paul
 * @date 02.12.21
 * Description here TODO
 */
#ifndef SYM_NOT_HPP
#define SYM_NOT_HPP

#include "../Expression.hpp"

namespace sym {
    template<Expression Expr>
    class Not {
      public:
        constexpr explicit Not(Expr expr);

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<Expression Expr_>
        constexpr friend auto getChildren(const sym::Not<Expr_> &notExpr) -> std::tuple<Expr_>;

        template<Expression Expr_>
        friend auto toString(const sym::Not<Expr_> &notExpr) -> std::string;

      private:
        [[no_unique_address]] Expr expr;
    };

    template<Expression Expr>
    constexpr Not<Expr>::Not(Expr expr) : expr{expr} {
    }

    template<Expression Expr>
    template<typename... Bindings>
    constexpr auto Not<Expr>::resolve(Bindings &&...bindings) const {
        return not expr.resolve(std::forward<Bindings>(bindings)...);
    }

    template<Expression Expr_>
    constexpr auto getChildren(const sym::Not<Expr_> &notExpr) -> std::tuple<Expr_> {
        return std::tuple{notExpr.expr};
    }

    template<Expression Expr_>
    auto toString(const sym::Not<Expr_> &notExpr) -> std::string {
        return "not (" + toString(notExpr.expr) + ")";
    }
} // namespace sym

#endif // SYM_NOT_HPP
