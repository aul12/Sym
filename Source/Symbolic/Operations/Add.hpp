/**
 * @file Add.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_ADD_HPP
#define SYM_ADD_HPP

#include "../Expression.hpp"
#include "../Variable.hpp"
#include "Sub.hpp"


namespace sym {
    namespace impl {
        template<typename T>
        struct IsAdd {
            static constexpr auto val = false;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    class Add {
      public:
        constexpr Add(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const;

        template<typename add, std::size_t ID>
        requires(impl::IsAdd<add>::val) friend constexpr auto gradient(const add &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Add<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    namespace impl {
        template<Expression lhs, Expression rhs>
        struct IsAdd<Add<lhs, rhs>> {
            static constexpr auto val = true;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    constexpr Add<Lhs, Rhs>::Add(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Add<Lhs, Rhs>::resolve(Bindings... bindings) const {
        return lhs.resolve(bindings...) + rhs.resolve(bindings...);
    }

    template<typename add, std::size_t ID>
    requires(impl::IsAdd<add>::val) constexpr auto gradient(const add &x, const Variable<ID> &d) {
        using ldiff = decltype(gradient(x.lhs, d));
        using rdiff = decltype(gradient(x.rhs, d));
        using dtype = Add<ldiff, rdiff>;

        return dtype{gradient(x.lhs, d), gradient(x.rhs, d)};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Add<Lhs_, Rhs_> &x) -> std::string {
        return "(" + toString(x.lhs) + "+" + toString(x.rhs) + ")";
    }
} // namespace sym

#endif // SYM_ADD_HPP
