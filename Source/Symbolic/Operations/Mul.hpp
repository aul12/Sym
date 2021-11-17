/**
 * @file Mul.hpp
 * @author paul
 * @date 05.09.20
 * Description here TODO
 */
#ifndef SYM_MUL_HPP
#define SYM_MUL_HPP

#include "../Expression.hpp"
#include "Add.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Mul {
      public:
        constexpr Mul(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, std::size_t ID>
        friend auto constexpr gradient(const Mul<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Mul<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Mul<Lhs, Rhs>::Mul(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Mul<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) * rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, std::size_t ID>
    constexpr auto gradient(const Mul<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        using lgrad = decltype(gradient(x.lhs, d));
        using rgrad = decltype(gradient(x.rhs, d));
        using lsum = Mul<lgrad, decltype(x.rhs)>;
        using rsum = Mul<rgrad, decltype(x.lhs)>;
        using dtype = Add<lsum, rsum>;

        return dtype{lsum{gradient(x.lhs, d), x.rhs}, rsum{gradient(x.rhs, d), x.lhs}};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Mul<Lhs_, Rhs_> &x) -> std::string {
        return toString(x.lhs) + "*" + toString(x.rhs);
    }
} // namespace sym

#endif // SYM_MUL_HPP
