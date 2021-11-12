/**
 * @file Sum.hpp
 * @author paul
 * @date 16.05.21
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_SUM_HPP
#define GRADIENTOPTIMIZATION_SUM_HPP

#include "Operators.hpp"

namespace sym {

    template<int min, int max, typename F>
    constexpr auto make_sum(F f) {
        static_assert(std::is_invocable_v<F, std::size_t>, "Body (F) of make_sum needs to be of form f(i)");
        using T = typename decltype(f(min))::type;

        if constexpr (max < min) {
            return make_sum<max, min>(f) * (-1);
        } else if constexpr (min == max) {
            return Constant<T>{0};
        } else {
            return f(min) + make_sum<min + 1, max>(f);
        }
    }
} // namespace sym

#endif // GRADIENTOPTIMIZATION_SUM_HPP
