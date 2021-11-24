/**
 * @file Div.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_DIV_HPP
#define SYM_DIV_HPP

#include "../RuntimeConstant.hpp"
#include "Add.hpp"
#include "Mul.hpp"

namespace sym {
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
        auto neg = RuntimeConstant{-1};
        return Div{Add{Mul{gradient(x.lhs, d), x.rhs}, Mul{Mul{x.lhs, gradient(x.rhs, d)}, neg}}, Mul{x.rhs, x.rhs}};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Div<Lhs_, Rhs_> &x) -> std::string {
        return "(" + sym::toString(x.lhs) + "/" + sym::toString(x.rhs) + ")";
    }

    template<typename Lhs_, typename Rhs_>
    constexpr auto getChildren(const Div<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }
} // namespace sym

#endif // SYM_DIV_HPP
