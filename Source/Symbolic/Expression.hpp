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
        { toString(t) } -> std::same_as<std::string>;
        //@TODO how resolve (needs to provide correct variables)
        {getChildren(t)};
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
