/**
 * @file Constant.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_CONSTANT_HPP
#define SYM_CONSTANT_HPP

#include "Expression.hpp"
#include "Variable.hpp"

namespace sym {
    template<std::size_t ID>
    class Variable;

    template<typename T>
    class Constant {
      public:
        explicit constexpr Constant(T val);

        template<typename... Bindings>
        constexpr auto resolve(Bindings...) const -> T;

        template<typename T_, std::size_t ID>
        friend constexpr auto gradient(const Constant<T_> &, const Variable<ID> &);

        template<typename T_>
        friend auto toString(const Constant<T_> &x) -> std::string;

      private:
        T val;
    };

    template<typename T>
    constexpr Constant<T>::Constant(T val) : val{val} {
    }

    template<typename T>
    template<typename... Bindings>
    constexpr auto Constant<T>::resolve(Bindings...) const -> T {
        return val;
    }

    template<typename T_, std::size_t ID>
    constexpr auto gradient(const Constant<T_> &, const Variable<ID> &) {
        return Constant<T_>{0};
    }

    template<typename T_>
    auto toString(const Constant<T_> &x) -> std::string {
        return std::to_string(x.val);
    }
} // namespace sym

#endif // SYM_CONSTANT_HPP
