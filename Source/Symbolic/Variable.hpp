/**
 * @file Variable.hpp
 * @author paul
 * @date 28.08.20
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_VARIABLE_HPP
#define GRADIENTOPTIMIZATION_VARIABLE_HPP

#include <memory>

#include "Constant.hpp"
#include "Expression.hpp"

namespace sym {
    template<typename T>
    class Constant;

    template<typename T>
    class Variable {
      public:
        using type = T;

        explicit constexpr Variable(T val = {});

        constexpr auto resolve() const -> T;

        constexpr void set(T t);

        template<typename T_>
        friend constexpr auto gradient(const Variable<T_> &x, const Variable<T_> &d);

        template<typename T_>
        friend auto toString(const Variable<T_> &x) -> std::string;

        static constexpr auto isConstant() -> bool;

      private:
        std::shared_ptr<T> val;
    };

    template<typename T>
    constexpr Variable<T>::Variable(T val) : val{std::make_shared<T>(std::move(val))} {
    }

    template<typename T>
    constexpr auto Variable<T>::resolve() const -> T {
        return *val;
    }

    template<typename T>
    constexpr void Variable<T>::set(T t) {
        *(this->val) = t;
    }

    template<typename T_>
    constexpr auto gradient(const Variable<T_> &x, const Variable<T_> &d) {
        return Constant<T_>(x.val.get() == d.val.get() ? 1 : 0);
    }

    template<typename T_>
    auto toString(const Variable<T_> &x) -> std::string {
        return "{" + std::to_string(x->val) + "}";
    }

    template<typename T>
    constexpr auto Variable<T>::isConstant() -> bool {
        return false;
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_VARIABLE_HPP
