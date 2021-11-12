/**
 * @file Sub.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_SUB_HPP
#define GRADIENTOPTIMIZATION_SUB_HPP

#include "Expression.hpp"
#include "Variable.hpp"


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
        using type = decltype(std::declval<Lhs>().resolve() - std::declval<Rhs>().resolve());

        constexpr Sub(Lhs lhs, Rhs rhs);

        constexpr auto resolve() const -> type;

        template<typename sub>
        requires(impl::IsSub<sub>::val) friend constexpr auto gradient(const sub &x,
                                                                       const Variable<typename sub::type> &d);

        template<typename sub>
        requires(impl::IsSub<sub>::val) friend auto toString(const sub &x) -> std::string;

        static constexpr auto isConstant() -> bool;

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
    constexpr auto Sub<Lhs, Rhs>::resolve() const -> type {
        return lhs.resolve() - rhs.resolve();
    }

    template<typename sub>
    requires(impl::IsSub<sub>::val) constexpr auto gradient(const sub &x, const Variable<typename sub::type> &d) {
        using ldiff = decltype(gradient(x.lhs, d));
        using rdiff = decltype(gradient(x.rhs, d));
        using dtype = Sub<ldiff, rdiff>;

        return dtype{gradient(x.lhs, d), gradient(x.rhs, d)};
    }

    template<typename sub>
    requires(impl::IsSub<sub>::val) auto toString(const sub &x) -> std::string {
        return "(" + x.lhs.toString() + "-" + x.rhs.toString() + ")";
    }

    template<Expression Lhs, Expression Rhs>
    constexpr auto Sub<Lhs, Rhs>::isConstant() -> bool {
        return Lhs::isConstant() and Rhs::isConstant();
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_SUB_HPP
