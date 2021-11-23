#include <benchmark/benchmark.h>

#include "Symbolic/Sum.hpp"
#include "Symbolic/Operations/Functions/Functions.hpp"

constexpr auto N = 500;

auto func = [](auto i, auto x, auto y, auto z) {
    return (x * x - std::exp(x * y + z * y + x) * z / (y + 7)) / (x * x * z + i + 12) + std::sin(x * y) / std::tan(z + i);
};

void complexResolve(benchmark::State &state) {
    sym::Variable<'x'> x;
    sym::Variable<'y'> y;
    sym::Variable<'z'> z;
    auto sum = sym::make_sum<0, N>([x, y, z](auto i) { return func(i, x, y, z); });

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double zVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(sum.resolve(x = xVal, y = yVal, z = zVal));
    }
}

BENCHMARK(complexResolve)->Unit(benchmark::TimeUnit::kMicrosecond);

void complexResolveNative(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double zVal = rand();
        state.ResumeTiming();
        double res = 0;
        for (auto i=0; i<N; ++i) {
            res += func(i, xVal, yVal, zVal);
        }
        benchmark::DoNotOptimize(res);
    }
}

BENCHMARK(complexResolveNative)->Unit(benchmark::TimeUnit::kMicrosecond);
