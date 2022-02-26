/**
 * @file Div.hpp
 * @author paul
 * @date 21.10.20
 * Description here TODO
 */
#ifndef SYM_DIV_HPP
#define SYM_DIV_HPP

#include "../Expression.hpp"
#include "../Simplifier/GradientSimplifcation.hpp"
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

        template<Expression Lhs_, Expression Rhs_, fixed_string ID>
        friend constexpr auto gradient(const Div<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Div<Lhs_, Rhs_> &x) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Div<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_>;

      private:
        [[no_unique_address]] Lhs lhs;
        [[no_unique_address]] Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Div<Lhs, Rhs>::Div(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Div<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) / rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, fixed_string ID>
    constexpr auto gradient(const Div<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        auto l_nom = _GRADIENT_SIMPLIFY(Mul{gradient(x.lhs, d), x.rhs});
        auto r_nom = _GRADIENT_SIMPLIFY(Mul{x.lhs, gradient(x.rhs, d)});
        auto nom = _GRADIENT_SIMPLIFY(Sub{l_nom, r_nom});
        auto denom = _GRADIENT_SIMPLIFY(Mul{x.rhs, x.rhs});
        return _GRADIENT_SIMPLIFY(Div{nom, denom});
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Div<Lhs_, Rhs_> &x) -> std::string {
        return "(" + toString(x.lhs) + "/" + toString(x.rhs) + ")";
    }

    template<Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Div<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }
} // namespace sym

#include "../RuntimeConstant.hpp"
#include "../Simplifier/CompileTime.hpp"
#include "Mul.hpp"
#include "Sub.hpp"

#endif // SYM_DIV_HPP
