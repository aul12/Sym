/**
 * @file Constant.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_RUNTIMECONSTANT_HPP
#define SYM_RUNTIMECONSTANT_HPP

#include "CompiletimeConstant.hpp"
#include "Expression.hpp"
#include "Variable.hpp"

namespace sym {
    template<std::size_t ID>
    class Variable;

    template<typename T>
    class RuntimeConstant {
      public:
        explicit constexpr RuntimeConstant(T val);

        template<typename... Bindings>
        constexpr auto resolve(Bindings...) const -> T;

        template<typename T_, std::size_t ID>
        friend constexpr auto gradient(const RuntimeConstant<T_> &, const Variable<ID> &);

        template<typename T_>
        friend auto toString(const RuntimeConstant<T_> &x) -> std::string;

        template<typename T_>
        constexpr friend auto getChildren(const RuntimeConstant<T_> &x) -> std::tuple<>;
      private:
        const T val;
    };

    template<typename T>
    constexpr RuntimeConstant<T>::RuntimeConstant(T val) : val{val} {
    }

    template<typename T>
    template<typename... Bindings>
    constexpr auto RuntimeConstant<T>::resolve(Bindings...) const -> T {
        return val;
    }

    template<typename T_, std::size_t ID>
    constexpr auto gradient(const RuntimeConstant<T_> &, const Variable<ID> &) {
        return CompiletimeConstant<int, 0>{};
    }

    template<typename T_>
    auto toString(const RuntimeConstant<T_> &x) -> std::string {
        return std::to_string(x.val);
    }
    template<typename T_>
    constexpr auto getChildren(const RuntimeConstant<T_> &x) -> std::tuple<> {
        return std::make_tuple();
    }
} // namespace sym

#endif // SYM_RUNTIMECONSTANT_HPP
