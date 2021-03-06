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
    constexpr auto mapTupleImpl(Tuple &&tuple, F &&f, std::index_sequence<indices...> /*seq*/) {
        return std::make_tuple(f(std::get<indices>(tuple))...);
    }

    template<typename F, typename... Ts>
    constexpr auto mapTuple(const std::tuple<Ts...> &tuple, F &&f) {
        using IndexSequence = std::make_index_sequence<sizeof...(Ts)>;
        return mapTupleImpl<F>(tuple, std::forward<F>(f), IndexSequence{});
    }

    template<typename TupleA, typename TupleB, std::size_t... indices>
    constexpr auto zipTupleImpl(const TupleA &tupleA, const TupleB &tupleB, std::index_sequence<indices...>) {
        return std::make_tuple(std::make_pair(std::get<indices>(tupleA), std::get<indices>(tupleB))...);
    }

    template<typename TupleA, typename TupleB>
    constexpr auto zipTuple(const TupleA &tupleA, const TupleB &tupleB) {
        static_assert(std::tuple_size_v<TupleA> == std::tuple_size_v<TupleB>);
        using IndexSequence = std::make_index_sequence<std::tuple_size_v<TupleA>>;
        return zipTupleImpl(tupleA, tupleB, IndexSequence{});
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

        template<fixed_string ID, Expression... Expressions_>
        constexpr friend auto gradient(const Vector<Expressions_...> &vec, const Variable<ID> &d);

        template<Expression Expr, fixed_string... IDs>
        constexpr friend auto gradient(const Expr &expr, const Vector<Variable<IDs>...> &ds);

      private:
        explicit Vector(const std::tuple<Expressions...> &expressions) : expressions{expressions} {
        }

        std::tuple<Expressions...> expressions;
    };

    template<typename Expr>
    struct IsVec {
        static constexpr auto val = false;
    };

    template<Expression... Expr>
    struct IsVec<Vector<Expr...>> {
        static constexpr auto val = true;
    };

    template<Expression Expr, fixed_string... IDs>
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
        auto delims = mapTuple(vec.expressions, [](Expression auto expression) {
            return IsVec<decltype(expression)>::val ? "\n" : ",";
        });
        auto stringsWithDelims = zipTuple(strings, delims);


        std::string result = "[";

        std::apply(
                [&result](auto &&...stringsWithDelims) {
                    ((result += stringsWithDelims.first + stringsWithDelims.second), ...);
                },
                stringsWithDelims);
        result[result.size() - 1] = ']';
        return result;
    }

    template<fixed_string ID, Expression... Expressions_>
    constexpr auto gradient(const Vector<Expressions_...> &vec, const Variable<ID> &d) {
        auto grads = mapTuple(vec.expressions, [&d](Expression auto expression) { return gradient(expression, d); });
        return Vector{grads};
    }

    template<std::size_t index, typename Container, fixed_string id, fixed_string... ids>
    constexpr auto bindVectorFromContainerImpl(Container &&container) {
        auto binding = std::make_tuple(Variable<id>{} = container[index]);
        if constexpr (sizeof...(ids) > 0) {
            return std::tuple_cat(binding, bindVectorFromContainerImpl<index + 1, Container, ids...>(
                                                   std::forward<Container>(container)));
        } else {
            return binding;
        }
    }

    template<typename Container, fixed_string... ids>
    constexpr auto bindVectorFromContainer(Vector<Variable<ids>...> /*sym*/, Container container) {
        return bindVectorFromContainerImpl<0, Container, ids...>(std::forward<Container>(container));
    }

} // namespace sym

#endif // SYM_VECTOR_HPP
