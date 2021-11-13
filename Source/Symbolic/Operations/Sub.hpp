/**
 * @file Sub.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_SUB_HPP
#define GRADIENTOPTIMIZATION_SUB_HPP

#include "../Expression.hpp"
#include "../Variable.hpp"


namespace sym {
    namespace impl {
        template<typename T>
        struct IsSub {
            static constexpr auto val = false;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    class Sub {
      public:
        constexpr Sub(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const;

        template<typename sub, std::size_t ID>
        requires(impl::IsSub<sub>::val) friend constexpr auto gradient(const sub &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Sub<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    namespace impl {
        template<Expression lhs, Expression rhs>
        struct IsSub<Sub<lhs, rhs>> {
            static constexpr auto val = true;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    constexpr Sub<Lhs, Rhs>::Sub(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Sub<Lhs, Rhs>::resolve(Bindings... bindings) const {
        return lhs.resolve(bindings...) - rhs.resolve(bindings...);
    }

    template<typename sub, std::size_t ID>
    requires(impl::IsSub<sub>::val) constexpr auto gradient(const sub &x, const Variable<ID> &d) {
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

#endif // GRADIENTOPTIMIZATION_SUB_HPP
