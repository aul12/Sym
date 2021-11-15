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
    constexpr auto mapTupleImpl(const std::tuple<Ts...> &tuple, F f) {
        if constexpr (index < sizeof...(Ts)) {
            auto &elem = std::get<index>(tuple);
            return std::apply([&elem, &f](auto &&...args) { return std::make_tuple(f(elem), args...); },
                              mapTupleImpl<index + 1>(tuple, f));
        } else {
            return std::tuple<>{};
        }
    }

    template<typename F, typename... Ts>
    constexpr auto mapTuple(const std::tuple<Ts...> &tuple, F f) {
        return mapTupleImpl<0>(tuple, f);
    }

    template<std::size_t index, typename T>
    constexpr auto flattenTupleImpl(const T &t) {
        return std::make_tuple(t);
    }

    template<std::size_t index, typename... Ts>
    constexpr auto flattenTupleImpl(const std::tuple<Ts...> &tuple) {
        if constexpr (index < sizeof...(Ts)) {
            auto elem = std::get<index>(tuple);
            return std::tuple_cat(flattenTupleImpl<0>(elem), flattenTupleImpl<index + 1>(tuple));
        } else {
            return std::tuple<>{};
        }
    }

    template<typename... Ts>
    constexpr auto flattenTuple(const std::tuple<Ts...> &tuple) {
        return flattenTupleImpl<0>(tuple);
    }

    template<Expression... Expressions>
    class Vector {
      public:
        explicit Vector(Expressions... expressions) : expressions{expressions...} {
        }

        template<typename... Bindings>
        constexpr auto resolve(Bindings... bindings) const {
            return mapTuple(expressions, [bindings...](Expression auto expression) {
                auto innerResult = expression.resolve(bindings...);
                return innerResult;
            });
        }

        template<typename T, typename... Bindings>
        constexpr auto resolveAs(Bindings... bindings) const -> T {
            auto resolved = resolve(bindings...);
            auto flattened = flattenTuple(resolved);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
            return std::apply([](auto... args) { return T{args...}; }, flattened);
#pragma GCC diagnostic pop
        }

        template<typename T, typename... Bindings>
        constexpr void resolveTo(T &t, Bindings... bindings) const {
            t = resolveAs<T>(bindings...);
        }

        template<Expression... Expressions_>
        friend auto toString(const Vector<Expressions_...> &vec) -> std::string;

        template<std::size_t ID, Expression... Expressions_>
        constexpr friend auto gradient(const Vector<Expressions_...> &vec, const sym::Variable<ID> &d);

        template<Expression Expr, std::size_t... IDs>
        constexpr friend auto gradient(const Expr &expr, const Vector<Variable<IDs>...> &ds);

      private:
        explicit Vector(const std::tuple<Expressions...> &expressions) : expressions{expressions} {
        }

        std::tuple<Expressions...> expressions;
    };

    template<Expression Expr, std::size_t... IDs>
    constexpr auto gradient(const Expr &expr, const Vector<Variable<IDs>...> &ds) {
        return Vector{mapTuple(ds.expressions, [&expr](auto &&d) { return gradient(expr, d); })};
    }


    template<Expression... Expressions_>
    auto toString(const Vector<Expressions_...> &vec) -> std::string {
        auto strings = mapTuple(vec.expressions, [](Expression auto expression) { return toString(expression); });
        std::string result = "[";
        std::apply([&result](auto &&...strings) { ((result += strings + ","), ...); }, strings);
        result[result.size() - 1] = ']';
        return result;
    }

    template<std::size_t ID, Expression... Expressions_>
    constexpr auto gradient(const Vector<Expressions_...> &vec, const sym::Variable<ID> &d) {
        auto grads = mapTuple(vec.expressions, [&d](Expression auto expression) { return gradient(expression, d); });
        return Vector{grads};
    }
} // namespace sym

#endif // SYM_VECTOR_HPP
