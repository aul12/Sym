/**
 * @file Mul.hpp
 * @author paul
 * @date 05.09.20
 * Description here TODO
 */
#ifndef SYM_MUL_HPP
#define SYM_MUL_HPP

#include "../Expression.hpp"
#include "../Simplifier/GradientSimplifcation.hpp"
#include "../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Add;

    template<Expression Lhs, Expression Rhs>
    class Mul {
      public:
        constexpr Mul(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, fixed_string ID>
        friend auto constexpr gradient(const Mul<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Mul<Lhs_, Rhs_> &x) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Mul<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_>;

      private:
        [[no_unique_address]] Lhs lhs;
        [[no_unique_address]] Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Mul<Lhs, Rhs>::Mul(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Mul<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) * rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, fixed_string ID>
    constexpr auto gradient(const Mul<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        auto l = _GRADIENT_SIMPLIFY(Mul{gradient(x.lhs, d), x.rhs});
        auto r = _GRADIENT_SIMPLIFY(Mul{gradient(x.rhs, d), x.lhs});

        return _GRADIENT_SIMPLIFY(Add{l, r});
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Mul<Lhs_, Rhs_> &x) -> std::string {
        return "(" + toString(x.lhs) + " * " + toString(x.rhs) + ")";
    }

    template<Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Mul<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }
} // namespace sym

#include "../Simplifier/CompileTime.hpp"
#include "Add.hpp"


#endif // SYM_MUL_HPP
