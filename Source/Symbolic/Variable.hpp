/**
 * @file Variable.hpp
 * @author paul
 * @date 28.08.20
 * Description here TODO
 */
#ifndef SYM_VARIABLE_HPP
#define SYM_VARIABLE_HPP

#include <memory>

#include "CompiletimeConstant.hpp"
#include "Expression.hpp"

namespace sym {
    template<typename T, T val>
    class CompiletimeConstant;

    template<std::size_t ID>
    class Variable;

    template<typename T, std::size_t ID_>
    class Binding {
      public:
        static constexpr auto ID = ID_;

      private:
        friend Variable<ID_>;

        constexpr explicit Binding(T val) : val{val} {
        }
        T val;
    };

    template<typename T>
    struct IsTuple {
        static constexpr auto val = false;
    };

    template<typename... Ts>
    struct IsTuple<std::tuple<Ts...>> {
        static constexpr auto val = true;
    };


    template<std::size_t ID, typename FirstBinding, typename... Bindings>
    constexpr auto findBinding(const FirstBinding &firstBinding, Bindings... bindings) {
        if constexpr (IsTuple<FirstBinding>::val) {
            return std::apply([&](auto &&...params) { return findBinding<ID>(params..., bindings...); }, firstBinding);
        } else if constexpr (FirstBinding::ID == ID) {
            return firstBinding;
        } else {
            static_assert(sizeof...(Bindings) > 0, "ID not found");
            return findBinding<ID, Bindings...>(bindings...);
        }
    }

    template<std::size_t ID>
    class Variable {
      public:
        template<std::size_t ID0, std::size_t ID1>
        friend constexpr auto gradient(const Variable<ID0> &x, const Variable<ID1> &d);

        template<std::size_t ID_>
        friend auto toString(const Variable<ID_> &x) -> std::string;

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const;

        template<typename T>
        [[nodiscard]] auto operator=(T val) const -> Binding<T, ID> {
            return Binding<T, ID>{val};
        }
    };

    template<std::size_t ID>
    template<typename... Bindings>
    constexpr auto Variable<ID>::resolve(Bindings... bindings) const {
        static_assert(sizeof...(Bindings) > 0, "No bindings specified!");
        return findBinding<ID, Bindings...>(bindings...).val;
    }

    template<std::size_t ID0, std::size_t ID1>
    constexpr auto gradient(const Variable<ID0> & /*x*/, const Variable<ID1> & /*d*/) {
        return CompiletimeConstant < int, ID0 == ID1 ? 1 : 0 > {};
    }

    template<std::size_t ID>
    auto toString(const Variable<ID> & /*x*/) -> std::string {
        return "{" + std::to_string(ID) + "}";
    }
} // namespace sym

#endif // SYM_VARIABLE_HPP
