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
    class Variable;

    template<typename T, std::size_t ID_>
    class Binding {
        friend Variable<T, ID_>;

      public:
        static constexpr auto ID = ID_;

      private:
        explicit Binding(T val) : val{val} {
        }
        T val;
    };

    template<std::size_t ID, typename FirstBinding, typename... Bindings>
    static constexpr auto findBinding(const FirstBinding &firstBinding, Bindings... bindings) {
        if constexpr (FirstBinding::ID == ID) {
            return firstBinding;
        } else {
            static_assert(sizeof...(Bindings) > 0, "ID not found");
            return findBinding<ID, Bindings...>(bindings...);
        }
    }

    template<typename T, std::size_t ID>
    class Variable {
        friend Binding<T, ID>;

      public:
        using type = T;

        template<typename T_, std::size_t ID0, std::size_t ID1>
        friend constexpr auto gradient(const Variable<T_, ID0> &x, const Variable<T_, ID1> &d);

        template<typename T_, std::size_t ID_>
        friend auto toString(const Variable<T_, ID_> &x) -> std::string;

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const -> T;

        [[nodiscard]] auto operator=(T val) -> Binding<T, ID> {
            return Binding<T, ID>{val};
        }
    };

    template<typename T, std::size_t ID>
    template<typename... Bindings>
    constexpr auto Variable<T, ID>::resolve(Bindings... bindings) const -> T {
        static_assert(sizeof...(Bindings) > 0, "No bindings specified!");
        return findBinding<ID, Bindings...>(bindings...).val;
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
