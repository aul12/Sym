/**
 * @file Constant.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_COMPILETIMECONSTANT_HPP
#define SYM_COMPILETIMECONSTANT_HPP

#include "Expression.hpp"
#include "Variable.hpp"

namespace sym {
    template<fixed_string ID>
    class Variable;

    template<typename T, T val_>
    class CompiletimeConstant {
        static constexpr auto val = val_;

      public:
        template<typename... Bindings>
        static constexpr auto resolve(Bindings...) -> T {
            return val_;
        }

        template<typename... Bindings>
        constexpr std::string resolve_named(const Bindings &...bindings) const;

        template<typename T_, T_ val0, fixed_string ID>
        friend constexpr auto gradient(const CompiletimeConstant<T_, val0> &, const Variable<ID> &);

        template<typename T_, T_ val0>
        friend auto toString(const CompiletimeConstant<T_, val0> &x) -> std::string;

        template<typename T_, T_ val0>
        constexpr friend auto getChildren(const CompiletimeConstant<T_, val0> &x) -> std::tuple<>;
    };

    template<typename T_, T_ val0>
    template<typename... Bindings>
    constexpr std::string CompiletimeConstant<T_, val0>::resolve_named(const Bindings &...) const {
        return std::to_string(val0);
    }

    template<typename T_, T_ val0, fixed_string ID>
    constexpr auto gradient(const CompiletimeConstant<T_, val0> &, const Variable<ID> &) {
        return CompiletimeConstant<T_, 0>{};
    }

    template<typename T_, T_ val0>
    auto toString(const CompiletimeConstant<T_, val0> &x) -> std::string {
        return std::to_string(x.val);
    }

    template<typename T_, T_ val0>
    constexpr auto getChildren(const CompiletimeConstant<T_, val0> & /*x*/) -> std::tuple<> {
        return std::make_tuple();
    }
} // namespace sym

#endif // SYM_COMPILETIMECONSTANT_HPP
