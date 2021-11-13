/**
 * @file Vector.hpp
 * @author paul
 * @date 13.11.21
 * Description here TODO
 */
#ifndef SYM_VECTOR_HPP
#define SYM_VECTOR_HPP

#include <array>

#include "Expression.hpp"
#include "Variable.hpp"

namespace sym {


    template<std::size_t index, typename F, typename... Ts>
    auto mapTupleImpl(const std::tuple<Ts...> &tuple, F f) {
        if constexpr (index < sizeof...(Ts)) {
            auto &elem = std::get<index>(tuple);
            return std::tuple_cat(std::tuple(f(elem)), mapTupleImpl<index + 1>(tuple, f));
        } else {
            return std::tuple<>{};
        }
    }

    template<typename F, typename... Ts>
    auto mapTuple(const std::tuple<Ts...> &tuple, F f) {
        return mapTupleImpl<0>(tuple, f);
    }

    template<Expression... Expressions>
    class Vector {
      public:
        explicit Vector(Expressions... expressions) : expressions{expressions...} {
        }

        template<typename... Bindings>
        auto resolve(Bindings... bindings) {
            return mapTuple(expressions,
                            [bindings...](Expression auto expression) { return expression.resolve(bindings...); });
        }

        template<typename T, typename... Bindings>
        auto resolveAs(Bindings... bindings) -> T {
            return std::apply([this](auto... args) { return T{args...}; }, resolve(bindings...));
        }

      private:
        std::tuple<Expressions...> expressions;
    };
} // namespace sym

#endif // SYM_VECTOR_HPP
