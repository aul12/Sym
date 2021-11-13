/**
 * @file Mul.hpp
 * @author paul
 * @date 05.09.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_MUL_HPP
#define GRADIENTOPTIMIZATION_MUL_HPP

#include "../Expression.hpp"
#include "Add.hpp"

namespace sym {
    namespace impl {
        template<typename T>
        struct IsMul {
            static constexpr auto val = false;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    class Mul {
      public:
        constexpr Mul(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const;

        template<typename mul, std::size_t ID>
        requires(impl::IsMul<mul>::val) friend auto constexpr gradient(const mul &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Mul<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    namespace impl {
        template<Expression lhs, Expression rhs>
        struct IsMul<Mul<lhs, rhs>> {
            static constexpr auto val = true;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    constexpr Mul<Lhs, Rhs>::Mul(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Mul<Lhs, Rhs>::resolve(Bindings... bindings) const {
        return lhs.resolve(bindings...) * rhs.resolve(bindings...);
    }

    template<typename mul, std::size_t ID>
    requires(impl::IsMul<mul>::val) constexpr auto gradient(const mul &x, const Variable<ID> &d) {
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

#endif // GRADIENTOPTIMIZATION_MUL_HPP
