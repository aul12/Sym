/**
 * @file Div.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_DIV_HPP
#define SYM_DIV_HPP

#include "../Expression.hpp"
#include "../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Sub;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<typename T>
    class RuntimeConstant;

    template<Expression Lhs, Expression Rhs>
    class Div {
      public:
        constexpr Div(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, std::size_t ID>
        friend constexpr auto gradient(const Div<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Div<Lhs_, Rhs_> &x) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Div<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_>;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Div<Lhs, Rhs>::Div(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Div<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) / rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, std::size_t ID>
    constexpr auto gradient(const Div<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        return Div{Sub{Mul{gradient(x.lhs, d), x.rhs}, Mul{x.lhs, gradient(x.rhs, d)}}, Mul{x.rhs, x.rhs}};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Div<Lhs_, Rhs_> &x) -> std::string {
        return "(" + sym::toString(x.lhs) + "/" + sym::toString(x.rhs) + ")";
    }

    template<Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Div<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }
} // namespace sym

#include "../RuntimeConstant.hpp"
#include "Mul.hpp"
#include "Sub.hpp"

#endif // SYM_DIV_HPP
