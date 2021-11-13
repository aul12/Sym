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
    template<typename T, std::size_t ID>
    class Variable;

    template<typename T>
    class Constant {
      public:
        using type = T;

        explicit constexpr Constant(T val);

        constexpr auto resolve() const -> T;

        template<typename T_, std::size_t ID>
        friend constexpr auto gradient(const Constant<T_> &, const Variable<T_, ID> &);

        template<typename T_>
        friend auto toString(const Constant<T_> &x) -> std::string;

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

    template<typename T_, std::size_t ID>
    constexpr auto gradient(const Constant<T_> &, const Variable<T_, ID> &) {
        return Constant<T_>{0};
    }

    template<typename T_>
    auto toString(const Constant<T_> &x) -> std::string {
        return std::to_string(x.val);
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_CONSTANT_HPP
