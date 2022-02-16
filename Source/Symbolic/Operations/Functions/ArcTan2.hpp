/**
 * @file ArcTan2.hpp
 * @author paul
 * @date 09.12.21
 * Description here TODO
 */
#ifndef SYM_ARCTAN2_HPP
#define SYM_ARCTAN2_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Div;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<Expression Lhs, Expression Rhs>
    class Add;

    template<Expression Lhs, Expression Rhs>
    class Sub;

    template<sym::Expression Lhs, sym::Expression Rhs>
    class ArcTan2 {
      public:
        explicit constexpr ArcTan2(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, fixed_string ID>
        constexpr friend auto gradient(const ArcTan2<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const ArcTan2<Lhs_, Rhs_> &x) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const ArcTan2<Lhs_, Rhs_> &tan) -> std::tuple<Lhs_, Rhs_>;

      private:
        Lhs lhs;
        Rhs rhs;
    };

    template<sym::Expression Lhs, sym::Expression Rhs>
    constexpr ArcTan2<Lhs, Rhs>::ArcTan2(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<sym::Expression Lhs, sym::Expression Rhs>
    template<typename... Bindings>
    constexpr auto ArcTan2<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return std::atan2(lhs.resolve(bindings...), rhs.resolve(bindings...));
    }

    template<sym::Expression Lhs_, sym::Expression Rhs_, fixed_string ID>
    constexpr auto gradient(const ArcTan2<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        // https://www.wolframalpha.com/input?i=d%2Fdx+atan2%28f%28x%29%2C+g%28x%29%29
        auto f = x.lhs;
        auto g = x.rhs;
        auto df = gradient(f, d);
        auto dg = gradient(g, d);
        return Div{Sub{Mul{g, df}, Mul{f, dg}}, Add{Mul{f, f}, Mul{g, g}}};
    }

    template<sym::Expression Lhs_, sym::Expression Rhs_>
    auto toString(const ArcTan2<Lhs_, Rhs_> &x) -> std::string {
        return "atan2(" + toString(x.lhs) + ", " + toString(x.rhs) + ")";
    }

    template<sym::Expression Lhs_, sym::Expression Rhs_>
    constexpr auto getChildren(const ArcTan2<Lhs_, Rhs_> &tan) -> std::tuple<Lhs_, Rhs_> {
        return std::tuple<Lhs_, Rhs_>(tan.lhs, tan.rhs);
    }
} // namespace sym

#include "../Add.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"

#endif // SYM_ARCTAN2_HPP
