/**
 * @file Pow.hpp
 * @author paul
 * @date 09.12.21
 * Description here TODO
 */
#ifndef SYM_POW_HPP
#define SYM_POW_HPP

#include <cmath>

#include "../../Expression.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<sym::Expression Lhs, sym::Expression Rhs>
    class Div;

    template<sym::Expression Lhs, sym::Expression Rhs>
    class Mul;

    template<sym::Expression Lhs, sym::Expression Rhs>
    class Sub;

    template<sym::Expression Lhs, sym::Expression Rhs>
    class Add;

    template<sym::Expression>
    class Log;

    template<sym::Expression>
    class Sqrt;

    template<sym::Expression Base, sym::Expression Exp>
    class Pow {
      public:
        explicit constexpr Pow(Base base, Exp exp);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<sym::Expression Base_, sym::Expression Exp_, fixed_string ID>
        constexpr friend auto gradient(const Pow<Base_, Exp_> &x, const Variable<ID> &d);

        template<sym::Expression Base_, sym::Expression Exp_>
        friend auto toString(const Pow<Base_, Exp_> &x) -> std::string;

        template<sym::Expression Base_, sym::Expression Exp_>
        constexpr friend auto getChildren(const Pow<Base_, Exp_> &cos) -> std::tuple<Base_, Exp_>;

      private:
        [[no_unique_address]] Base base;
        [[no_unique_address]] Exp exp;
    };

    template<sym::Expression Base, sym::Expression Exp>
    constexpr Pow<Base, Exp>::Pow(Base base, Exp exp) : base{base}, exp{exp} {
    }

    template<sym::Expression Base, sym::Expression Exp>
    template<typename... Bindings>
    constexpr auto Pow<Base, Exp>::resolve(const Bindings &...bindings) const {
        return std::pow(base.resolve(bindings...), exp.resolve(bindings...));
    }

    template<sym::Expression Base_, sym::Expression Exp_, fixed_string ID>
    constexpr auto gradient(const Pow<Base_, Exp_> &x, const Variable<ID> &d) {
        return Mul{Pow{x.base, Sub{x.exp, CompiletimeConstant<int, 1>{}}},
                   Add{Mul{x.exp, gradient(x.base, d)}, Mul{Mul{x.base, Log{x.base}}, gradient(x.exp, d)}}};
    }

    template<sym::Expression Base_, sym::Expression Exp_>
    auto toString(const Pow<Base_, Exp_> &x) -> std::string {
        return "(" + toString(x.base) + ")^(" + toString(x.exp) + ")";
    }

    template<sym::Expression Base_, sym::Expression Exp_>
    constexpr auto getChildren(const Pow<Base_, Exp_> &pow) -> std::tuple<Base_, Exp_> {
        return std::tuple<Base_, Exp_>(pow.base, pow.exp);
    }
} // namespace sym

#include "../Add.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Log.hpp"
#include "Sqrt.hpp"

#endif // SYM_POW_HPP
