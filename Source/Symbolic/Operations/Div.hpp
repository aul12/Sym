/**
 * @file Div.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_DIV_HPP
#define SYM_DIV_HPP

#include "../Constant.hpp"
#include "Add.hpp"
#include "Mul.hpp"

namespace sym {
    namespace impl {
        template<typename T>
        struct IsDiv {
            static constexpr auto val = false;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    class Div {
      public:
        constexpr Div(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const;

        template<typename div, std::size_t ID>
        requires(impl::IsDiv<div>::val) friend constexpr auto gradient(const div &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Div<Lhs_, Rhs_> &x) -> std::string;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    namespace impl {
        template<Expression lhs, Expression rhs>
        struct IsDiv<Div<lhs, rhs>> {
            static constexpr auto val = true;
        };
    } // namespace impl

    template<Expression Lhs, Expression Rhs>
    constexpr Div<Lhs, Rhs>::Div(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Div<Lhs, Rhs>::resolve(Bindings... bindings) const {
        return lhs.resolve(bindings...) / rhs.resolve(bindings...);
    }

    template<typename div, std::size_t ID>
    requires(impl::IsDiv<div>::val) constexpr auto gradient(const div &x, const Variable<ID> &d) {
        auto neg = Constant{-1};
        return Div{Add{Mul{gradient(x.lhs, d), x.rhs}, Mul{Mul{x.lhs, gradient(x.rhs, d)}, neg}}, Mul{x.rhs, x.rhs}};
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Div<Lhs_, Rhs_> &x) -> std::string {
        return "(" + x.lhs.toString() + "/" + x.rhs.toString() + ")";
    }
} // namespace sym

#endif // SYM_DIV_HPP
