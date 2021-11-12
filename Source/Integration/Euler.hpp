/**
 * @file Euler.hpp.h
 * @author paul
 * @date 01.02.21
 * Description here TODO
 */
#ifndef GRADIENTOPTIMIZATION_EULER_HPP
#define GRADIENTOPTIMIZATION_EULER_HPP

#include "../Symbolic/Add.hpp"
#include "../Symbolic/Expression.hpp"
#include "../Symbolic/Operators.hpp"
#include "../Symbolic/Variable.hpp"

namespace sym::integrate {
    namespace impl {
        template<std::size_t steps, typename F, sym::Expression X, sym::Expression Acc>
        constexpr auto eulerStep(F f, X x, Acc acc, typename X::type delta) {
            if constexpr (steps > 0) {
                auto nextX = x + delta;
                auto updatedAcc = acc + delta * f(x);
                return eulerStep<steps - 1, F, decltype(nextX), decltype(updatedAcc)>(f, nextX, updatedAcc, delta);
            } else {
                return acc;
            }
        }
    } // namespace impl

    namespace helper {
        template<typename T>
        consteval auto getSteps(T min, T max, T delta) {
            return static_cast<std::size_t>((max - min) / delta);
        }
    } // namespace helper

    struct make_euler_impl {
        template<std::size_t steps, typename F, sym::Expression Min>
        auto make(F f, Min min, typename Min::type delta) {
            using T = typename Min::type;
            auto acc = sym::Constant<T>{0};
            return impl::eulerStep<steps>(f, min, acc, delta);
        }

        template<std::size_t steps, typename F, typename T>
        auto make(F f, T min, T delta) {
            auto minConst = sym::Constant<T>{min};
            return make<steps, F, decltype(minConst)>(f, minConst, delta);
        }
    } make_euler;

} // namespace sym::integrate

#endif // GRADIENTOPTIMIZATION_EULER_HPP
