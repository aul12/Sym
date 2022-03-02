/**
 * @file Variable.hpp
 * @author paul
 * @date 28.08.20
 * Description here TODO
 */
#ifndef SYM_VARIABLE_HPP
#define SYM_VARIABLE_HPP

#include <memory>

#include "Expression.hpp"
#include "FixedString.hpp"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

/**
 * Define a variable with the same name and identifier
 */
#define SYM_VARIABLE(name) sym::Variable< #name > name

/**
 * Define a variable with automatic name based on file name and line number
 */
#define SYM_A_VARIABLE sym::Variable<__FILE__ ":" STRINGIZE(__LINE__)>

namespace sym {
    template<typename T, T val>
    class CompiletimeConstant;

    template<fixed_string ID>
    class Variable;

    template<typename T, fixed_string ID_>
    class Binding {
      public:
        static constexpr auto ID = ID_;

      private:
        friend Variable<ID_>;

        constexpr explicit Binding(T val) : val{val} {
        }
        T val;
    };

    template<std::size_t index, fixed_string ID, typename T>
    struct findBinding {
        static constexpr std::size_t val = -1;
    };

    template<std::size_t index, fixed_string ID, typename FirstBinding, typename... Bindings>
    struct findBinding<index, ID, std::tuple<FirstBinding, Bindings...>> {
        static constexpr std::size_t followingVal = findBinding<index + 1, ID, std::tuple<Bindings...>>::val;
        static constexpr std::size_t val = FirstBinding::ID == ID ? (followingVal == -1 ? index : -2) : followingVal;
    };

    template<typename T>
    constexpr auto wrapInTuple(T &&t) {
        return std::make_tuple(std::forward<T>(t));
    }

    template<typename... Ts>
    constexpr auto wrapInTuple(const std::tuple<Ts...> &tuple) {
        return tuple;
    }

    template<fixed_string ID>
    class Variable {
      public:
        template<fixed_string ID0, fixed_string ID1>
        friend constexpr auto gradient(const Variable<ID0> &x, const Variable<ID1> &d);

        template<fixed_string ID_>
        friend auto toString(const Variable<ID_> &x) -> std::string;

        template<fixed_string ID_>
        constexpr friend auto getChildren(const Variable<ID_> &x) -> std::tuple<>;

        template<typename... Bindings>
        constexpr auto resolve(Bindings &&...bindings) const;

        template<typename... Bindings>
        constexpr auto resolve(const std::tuple<Bindings...> &tuple) const;

        template<typename T>
        [[nodiscard]] auto operator=(T val) const -> Binding<T, ID> { // NOLINT
            return Binding<T, ID>{val};
        }
    };

    template<fixed_string ID>
    template<typename... Bindings>
    constexpr auto Variable<ID>::resolve(Bindings &&...bindings) const {
        auto tuple = std::tuple_cat(wrapInTuple(std::forward<Bindings>(bindings))...);
        constexpr auto index = findBinding<0, ID, decltype(tuple)>::val;
        static_assert(index != -1, "No binding found!");
        static_assert(index != -2, "Multiple bindings for same variable found!");
        return std::get<index>(tuple).val;
    }

    template<fixed_string ID>
    template<typename... Bindings>
    constexpr auto Variable<ID>::resolve(const std::tuple<Bindings...> &tuple) const {
        constexpr auto index = findBinding<0, ID, std::tuple<Bindings...>>::val;
        static_assert(index != -1, "No binding found!");
        static_assert(index != -2, "Multiple bindings for same variable found!");
        return std::get<index>(tuple).val;
    }

    template<fixed_string ID0, fixed_string ID1>
    constexpr auto gradient(const Variable<ID0> & /*x*/, const Variable<ID1> & /*d*/) {
        return CompiletimeConstant < int, ID0 == ID1 ? 1 : 0 > {};
    }

    template<fixed_string ID>
    auto toString(const Variable<ID> & /*x*/) -> std::string {
        return std::string{ID.data};
    }

    template<fixed_string ID_>
    constexpr auto getChildren(const Variable<ID_> & /*x*/) -> std::tuple<> {
        return std::make_tuple();
    }
} // namespace sym

#include "CompiletimeConstant.hpp"

#endif // SYM_VARIABLE_HPP
