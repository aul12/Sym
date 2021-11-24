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

    template<std::size_t index, std::size_t ID, typename T>
    struct findBinding {
        static constexpr std::size_t val = -1;
    };

    template<std::size_t index, std::size_t ID, typename FirstBinding, typename... Bindings>
    struct findBinding<index, ID, std::tuple<FirstBinding, Bindings...>> {
        static constexpr std::size_t val =
                FirstBinding::ID == ID ? index : findBinding<index + 1, ID, std::tuple<Bindings...>>::val;
    };

    template<typename T>
    auto wrapInTuple(T &&t) {
        return std::make_tuple(std::forward<T>(t));
    }

    template<typename... Ts>
    auto wrapInTuple(const std::tuple<Ts...> &tuple) {
        return tuple;
    }

    template<std::size_t ID>
    class Variable {
      public:
        template<std::size_t ID0, std::size_t ID1>
        friend constexpr auto gradient(const Variable<ID0> &x, const Variable<ID1> &d);

        template<std::size_t ID_>
        friend auto toString(const Variable<ID_> &x) -> std::string;

        template<std::size_t ID_>
        constexpr friend auto getChildren(const Variable<ID_> &x) -> std::tuple<>;

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<typename... Bindings>
        constexpr auto resolve(const std::tuple<Bindings...> &tuple) const;

        template<typename T>
        [[nodiscard]] auto operator=(T val) const -> Binding<T, ID> {
            return Binding<T, ID>{val};
        }
    };

    template<std::size_t ID>
    template<typename... Bindings>
    constexpr auto Variable<ID>::resolve(Bindings &&...bindings) const {
        auto tuple = std::tuple_cat(wrapInTuple(std::forward<Bindings>(bindings))...);
        constexpr auto index = findBinding<0, ID, decltype(tuple)>::val;
        static_assert(index != -1, "No binding found!");
        return std::get<index>(tuple).val;
    }

    template<std::size_t ID>
    template<typename... Bindings>
    constexpr auto Variable<ID>::resolve(const std::tuple<Bindings...> &tuple) const {
        constexpr auto index = findBinding<0, ID, std::tuple<Bindings...>>::val;
        static_assert(index != -1, "No binding found!");
        return std::get<index>(tuple).val;
    }

    template<std::size_t ID0, std::size_t ID1>
    constexpr auto gradient(const Variable<ID0> & /*x*/, const Variable<ID1> & /*d*/) {
        return CompiletimeConstant < int, ID0 == ID1 ? 1 : 0 > {};
    }

    template<std::size_t ID>
    auto toString(const Variable<ID> & /*x*/) -> std::string {
        return "{" + std::to_string(ID) + "}";
    }

    template<std::size_t ID_>
    constexpr auto getChildren(const Variable<ID_> & /*x*/) -> std::tuple<> {
        return std::make_tuple();
    }
} // namespace sym

#endif // SYM_VARIABLE_HPP
