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
    template<typename F, typename Tuple, std::size_t... indices>
    constexpr auto mapTupleImpl(Tuple &&tuple, F &&f, std::integer_sequence<std::size_t, indices...> /*seq*/) {
        return std::make_tuple(f(std::get<indices>(tuple))...);
    }

    template<typename F, typename... Ts>
    constexpr auto mapTuple(const std::tuple<Ts...> &tuple, F &&f) {
        using IndexSequence = std::make_index_sequence<sizeof...(Ts)>;
        return mapTupleImpl<F>(tuple, std::forward<F>(f), IndexSequence{});
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

    template<std::size_t index, typename T, typename Tuple>
    constexpr auto resolveToImpl(T &t, Tuple &&tuple) {
        t[index] = std::get<index>(tuple);
        if constexpr (index + 1 < std::tuple_size_v<Tuple>) {
            resolveToImpl<index + 1>(t, std::forward<Tuple>(tuple));
        }
    }

    template<Expression... Expressions>
    class Vector {
      public:
        explicit Vector(Expressions... expressions) : expressions{expressions...} {
        }

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const {
            return mapTuple(expressions, [&bindings...](Expression auto expression) {
                auto innerResult = expression.resolve(bindings...);
                return innerResult;
            });
        }

        template<typename T, typename... Bindings>
        constexpr auto resolveAs(Bindings &&...bindings) const -> T {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
            return std::apply([](auto... args) { return T{args...}; },
                              flattenTuple(resolve(std::forward<Bindings>(bindings)...)));
#pragma GCC diagnostic pop
        }

        template<typename T, typename... Bindings>
        constexpr void resolveTo(T &t, Bindings &&...bindings) const {
            resolveToImpl<0>(t, flattenTuple(resolve(std::forward<Bindings>(bindings)...)));
        }

        template<Expression... Expressions_>
        friend auto toString(const Vector<Expressions_...> &vec) -> std::string;

        template<Expression... Expressions_>
        constexpr friend auto getChildren(const Vector<Expressions_...> &vec) -> std::tuple<Expressions_...>;

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
    constexpr auto getChildren(const Vector<Expressions_...> &vec) -> std::tuple<Expressions_...> {
        return vec.expressions;
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

    template<std::size_t index, typename Container, std::size_t id, std::size_t... ids>
    constexpr auto bindVectorFromContainerImpl(Container &&container) {
        auto binding = std::make_tuple(sym::Variable<id>{} = container[index]);
        if constexpr (sizeof...(ids) > 0) {
            return std::tuple_cat(binding, bindVectorFromContainerImpl<index + 1, Container, ids...>(
                                                   std::forward<Container>(container)));
        } else {
            return binding;
        }
    }

    template<typename Container, std::size_t... ids>
    constexpr auto bindVectorFromContainer(Vector<Variable<ids>...> /*sym*/, Container container) {
        return bindVectorFromContainerImpl<0, Container, ids...>(std::forward<Container>(container));
    }

} // namespace sym

#endif // SYM_VECTOR_HPP
