/**
 * @file Add.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_ADD_HPP
#define GRADIENTOPTIMIZATION_ADD_HPP

#include "Expression.hpp"
#include "Sub.hpp"
#include "Variable.hpp"


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
        using type = decltype(std::declval<Lhs>().resolve() + std::declval<Rhs>().resolve());

        constexpr Add(Lhs lhs, Rhs rhs);

        constexpr auto resolve() const -> type;

        template<typename add>
        requires(impl::IsAdd<add>::val) friend constexpr auto gradient(const add &x,
                                                                       const Variable<typename add::type> &d);

        template<typename add>
        requires(impl::IsAdd<add>::val) friend auto toString(const add &x) -> std::string;

        static constexpr auto isConstant() -> bool;

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
    constexpr auto Add<Lhs, Rhs>::resolve() const -> type {
        return lhs.resolve() + rhs.resolve();
    }

    template<typename add>
    requires(impl::IsAdd<add>::val) constexpr auto gradient(const add &x, const Variable<typename add::type> &d) {
        using ldiff = decltype(gradient(x.lhs, d));
        using rdiff = decltype(gradient(x.rhs, d));
        using dtype = Add<ldiff, rdiff>;

        return dtype{gradient(x.lhs, d), gradient(x.rhs, d)};
    }

    template<typename add>
    requires(impl::IsAdd<add>::val) auto toString(const add &x) {
        return "(" + x.toString() + "+" + x.toString() + ")";
    }
    template<Expression Lhs, Expression Rhs>
    constexpr auto Add<Lhs, Rhs>::isConstant() -> bool {
        return Lhs::isConstant() and Rhs::isConstant();
    }


} // namespace sym

#endif // GRADIENTOPTIMIZATION_ADD_HPP
