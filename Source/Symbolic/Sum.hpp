/**
 * @file Sum.hpp
 * @author paul
 * @date 16.05.21
 * Description here TODO
 */
#ifndef SYM_SUM_HPP
#define SYM_SUM_HPP

#include "Operators.hpp"

namespace sym {

    template<int min, int max, typename F>
    constexpr auto make_sum(F f) {
        static_assert(std::is_invocable_v<F, std::size_t>, "Body (F) of make_sum needs to be of form f(i)");
        if constexpr (max < min) {
            return make_sum<max, min>(f) * (-1);
        } else if constexpr (min == max) {
            return Constant{0};
        } else {
            return f(min) + make_sum<min + 1, max>(f);
        }
    }
} // namespace sym

#endif // SYM_SUM_HPP
