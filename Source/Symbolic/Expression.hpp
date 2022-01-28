/**
 * @file Expression.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_EXPRESSION_HPP
#define SYM_EXPRESSION_HPP

#include <concepts>
#include <string>
#include <tuple>

namespace sym {
    template<typename T>
    constexpr auto nonCvRefType = std::is_same_v<T, std::remove_cvref_t<T>>;

    template<typename Tuple, std::size_t... ids>
    constexpr auto AllNonCvRefTypeImpl(std::index_sequence<ids...>) {
        return (... and nonCvRefType<std::tuple_element_t<ids, Tuple>>);
    }

    template<typename... Ts>
    constexpr auto AllNonCvRefType(std::tuple<Ts...>) {
        constexpr auto allNonCvRef = AllNonCvRefTypeImpl<std::tuple<Ts...>>(std::index_sequence_for<Ts...>{});
        if constexpr (allNonCvRef) {
            return std::true_type{};
        } else {
            return std::false_type{};
        }
    }

    template<typename T>
    concept Expression = requires(const T t) {
        { toString(t) } -> std::same_as<std::string>;
        //@TODO how resolve (needs to provide correct variables)
        { AllNonCvRefType(getChildren(t)) } -> std::same_as<std::true_type>;
    };


    template<typename T>
    struct IsExpression {
        static constexpr auto val = false;
    };

    template<Expression expr>
    struct IsExpression<expr> {
        static constexpr auto val = true;
    };
} // namespace sym

#endif // SYM_EXPRESSION_HPP
