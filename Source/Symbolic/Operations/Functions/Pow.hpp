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
#include "../../Simplifier/GradientSimplifcation.hpp"
#include "../../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Div;

    template<Expression Lhs, Expression Rhs>
    class Mul;

    template<Expression Lhs, Expression Rhs>
    class Sub;

    template<Expression Lhs, Expression Rhs>
    class Add;

    template<Expression>
    class Log;

    template<Expression>
    class Sqrt;

    template<Expression Base, Expression Exp>
    class Pow {
      public:
        explicit constexpr Pow(Base base, Exp exp);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Base_, Expression Exp_, fixed_string ID>
        constexpr friend auto gradient(const Pow<Base_, Exp_> &x, const Variable<ID> &d);

        template<Expression Base_, Expression Exp_>
        friend auto toString(const Pow<Base_, Exp_> &x) -> std::string;

        template<Expression Base_, Expression Exp_>
        constexpr friend auto getChildren(const Pow<Base_, Exp_> &cos) -> std::tuple<Base_, Exp_>;

      private:
        [[no_unique_address]] Base base;
        [[no_unique_address]] Exp exp;
    };

    template<Expression Base, Expression Exp>
    constexpr Pow<Base, Exp>::Pow(Base base, Exp exp) : base{base}, exp{exp} {
    }

    template<Expression Base, Expression Exp>
    template<typename... Bindings>
    constexpr auto Pow<Base, Exp>::resolve(const Bindings &...bindings) const {
        return std::pow(base.resolve(bindings...), exp.resolve(bindings...));
    }

    template<Expression Base_, Expression Exp_, fixed_string ID>
    constexpr auto gradient(const Pow<Base_, Exp_> &x, const Variable<ID> &d) {
        auto exp_minus_1 = _GRADIENT_SIMPLIFY(Sub{x.exp, CompiletimeConstant<int, 1>{}});
        auto x_pow_exp_minus_1 = _GRADIENT_SIMPLIFY(Pow{x.base, exp_minus_1});

        auto x_log_x = _GRADIENT_SIMPLIFY(Mul{x.base, Log{x.base}});
        auto x_log_x_dexp = _GRADIENT_SIMPLIFY(Mul{x_log_x, gradient(x.exp, d)});
        auto x_dx = _GRADIENT_SIMPLIFY(Mul{x.exp, gradient(x.base, d)});

        auto add = _GRADIENT_SIMPLIFY(Add{x_dx, x_log_x_dexp});

        return _GRADIENT_SIMPLIFY(Mul{x_pow_exp_minus_1, add});
    }

    template<Expression Base_, Expression Exp_>
    auto toString(const Pow<Base_, Exp_> &x) -> std::string {
        return "(" + toString(x.base) + ")^(" + toString(x.exp) + ")";
    }

    template<Expression Base_, Expression Exp_>
    constexpr auto getChildren(const Pow<Base_, Exp_> &pow) -> std::tuple<Base_, Exp_> {
        return std::tuple<Base_, Exp_>(pow.base, pow.exp);
    }
} // namespace sym

#include "../../Simplifier/CompileTime.hpp"
#include "../Add.hpp"
#include "../Div.hpp"
#include "../Mul.hpp"
#include "../Sub.hpp"
#include "Log.hpp"
#include "Sqrt.hpp"

#endif // SYM_POW_HPP
