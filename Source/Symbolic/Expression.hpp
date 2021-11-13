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

namespace sym {
    template<typename T>
    concept Expression = requires(const T t) {
        //@TODO requires polymorphic resolve
        //{ t.resolve() } -> std::same_as<typename T::type>;
        true;
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
