/**
 * @file Constant.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_CONSTANT_HPP
#define GRADIENTOPTIMIZATION_CONSTANT_HPP

#include "Expression.hpp"
#include "Variable.hpp"

namespace sym {
    template<typename>
    class Variable;

    template<typename T>
    class Constant {
      public:
        using type = T;

        explicit constexpr Constant(T val);

        constexpr auto resolve() const -> T;

        template<typename T_>
        friend constexpr auto gradient(const Constant<T_> &, const Variable<T_> &);

        template<typename T_>
        friend auto toString(const Constant<T_> &x) -> std::string;

        static constexpr auto isConstant() -> bool;

      private:
        T val;
    };

    template<typename T>
    constexpr Constant<T>::Constant(T val) : val{val} {
    }

    template<typename T>
    constexpr auto Constant<T>::resolve() const -> T {
        return val;
    }

    template<typename T_>
    constexpr auto gradient(const Constant<T_> &, const Variable<T_> &) {
        return Constant<T_>{0};
    }

    template<typename T_>
    auto toString(const Constant<T_> &x) -> std::string {
        return std::to_string(x.val);
    }

    template<typename T>
    constexpr auto Constant<T>::isConstant() -> bool {
        return true;
    }

} // namespace sym

#endif // GRADIENTOPTIMIZATION_CONSTANT_HPP
