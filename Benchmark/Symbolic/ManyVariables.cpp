#include <Symbolic/Operators.hpp>
#include <Symbolic/Variable.hpp>
#include <benchmark/benchmark.h>

constexpr auto N = 10;

template<sym::fixed_string... IDs>
auto getVariableTuple(std::index_sequence<IDs...>) {
    return std::make_tuple(sym::Variable<IDs>{}...);
}

template<sym::fixed_string... IDs>
auto getSum(std::tuple<sym::Variable<IDs>...>) {
    return (... + sym::Variable<IDs>{});
}

template<std::size_t index, sym::fixed_string... IDs>
auto getBinding(std::tuple<sym::Variable<IDs>...> tuple) {
    auto currBinding = (std::get<index>(tuple) = static_cast<double>(rand()));
    if constexpr (index < sizeof...(IDs) - 1) {
        return std::tuple_cat(std::make_tuple(currBinding), getBinding<index + 1, IDs...>(tuple));
    } else {
        return std::make_tuple(currBinding);
    }
}

/*
 * TODO: Fix make_index_sequence
void manyVariableAsTuple(benchmark::State &state) {
    auto variables = getVariableTuple(std::make_index_sequence<N>());
    auto expr = getSum(variables);

    for (auto _ : state) {
        state.PauseTiming();
        auto binding = getBinding<0>(variables);
        state.ResumeTiming();
        benchmark::DoNotOptimize(expr.resolve(binding));
    }
}
BENCHMARK(manyVariableAsTuple);

void manyVariableAsVariadic(benchmark::State &state) {
    auto variables = getVariableTuple(std::make_index_sequence<N>());
    auto expr = getSum(variables);

    for (auto _ : state) {
        state.PauseTiming();
        auto binding = getBinding<0>(variables);
        std::apply(
                [&state, expr](auto &&...params) {
                    state.ResumeTiming();
                    benchmark::DoNotOptimize(expr.resolve(params...));
                },
                binding);
    }
}
BENCHMARK(manyVariableAsVariadic);
*/

void manyVariableNative(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::array<double, N> arr{};
        for (auto &elem : arr) {
            elem = rand();
        }
        state.ResumeTiming();
        double sum = 0;
        for (auto elem : arr) {
            sum += elem;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(manyVariableNative);
