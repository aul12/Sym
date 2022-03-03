/**
 * @file Greater.hpp
 * @author paul
 * @date 17.11.21
 * Description here TODO
 */
#ifndef SYM_GREATER_HPP
#define SYM_GREATER_HPP

#include <tuple>

#include "../Expression.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Greater {
      public:
        constexpr explicit Greater(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
        }

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Greater<Lhs_, Rhs_> &greater) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Greater<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_>;

      private:
        [[no_unique_address]] Lhs lhs;
        [[no_unique_address]] Rhs rhs;
    };


    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Greater<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) > rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Greater<Lhs_, Rhs_> &greater) -> std::string {
        return "(" + toString(greater.lhs) + ")>(" + toString(greater.rhs) + ")";
    }

    template<Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Greater<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }

} // namespace sym

#endif // SYM_GREATER_HPP
