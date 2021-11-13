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

    template<typename T, std::size_t ID>
    class Variable {
      public:
        using type = T;

        T t;

        explicit Variable(T t) : t{t} {
        }

        template<typename T_, std::size_t ID0, std::size_t ID1>
        friend constexpr auto gradient(const Variable<T_, ID0> &x, const Variable<T_, ID1> &d);

        template<typename T_, std::size_t ID_>
        friend auto toString(const Variable<T_, ID_> &x) -> std::string;

        constexpr auto resolve() const -> T;
    };

    template<typename T, std::size_t ID>
    constexpr auto Variable<T, ID>::resolve() const -> T {
        return t;
    }

    template<typename T_, std::size_t ID0, std::size_t ID1>
    constexpr auto gradient(const Variable<T_, ID0> & /*x*/, const Variable<T_, ID1> & /*d*/) {
        return Constant<T_>(ID0 == ID1 ? 1 : 0);
    }

    template<typename T_, std::size_t ID>
    auto toString(const Variable<T_, ID> &x) -> std::string {
        return "{" + std::to_string(ID) + "}";
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_VARIABLE_HPP
