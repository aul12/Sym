/**
 * @file Sub.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_SUB_HPP
#define SYM_SUB_HPP

#include "../Expression.hpp"
#include "../Variable.hpp"


namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Sub {
      public:
        constexpr Sub(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, std::size_t ID>
        friend constexpr auto gradient(const Sub<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Sub<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Sub<Lhs, Rhs>::Sub(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Sub<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) - rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, std::size_t ID>
    constexpr auto gradient(const Sub<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        using ldiff = decltype(gradient(x.lhs, d));
        using rdiff = decltype(gradient(x.rhs, d));
        using dtype = Sub<ldiff, rdiff>;

        return dtype{gradient(x.lhs, d), gradient(x.rhs, d)};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Sub<Lhs_, Rhs_> &x) -> std::string {
        return "(" + toString(x.lhs) + "-" + toString(x.rhs) + ")";
    }
} // namespace sym

#endif // SYM_SUB_HPP
