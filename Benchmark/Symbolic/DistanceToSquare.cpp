#include <benchmark/benchmark.h>
#include <iostream>

#include "Symbolic/Operators.hpp"
#include "Symbolic/Operations/Functions/Functions.hpp"
#include "Symbolic/Vector.hpp"

auto distanceToSquare(auto x, auto y, auto centreX, auto centreY, auto width, auto height) {
    auto deltaX = std::abs(x - centreX);
    auto deltaY = std::abs(y - centreY);
    auto xDist = std::max(0.0, deltaX - (width / 2.0));
    auto yDist = std::max(0.0, deltaY - (height / 2.0));
    return xDist * xDist + yDist * yDist;
}

void distToSquareSym(benchmark::State &state) {
    sym::Variable<'x'> x;
    sym::Variable<'y'> y;

    auto centreX = 10;
    auto centreY = 12;
    auto width = 3;
    auto height = 4;

    auto dist = distanceToSquare(x, y, centreX, centreY, width, height);

    sym::Variable<'z'> z;
    auto d_dist = sym::gradient(dist, z);
    auto dd_dist = sym::gradient(d_dist, z);

    std::cout << sym::toString(d_dist) << std::endl;

    std::cout << sym::toString(dd_dist) << std::endl;

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(dist.resolve(x = xVal, y = yVal));
    }
}

BENCHMARK(distToSquareSym);

void distToSquareNative(benchmark::State &state) {
    auto centreX = 10;
    auto centreY = 12;
    auto width = 3;
    auto height = 4;

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        state.ResumeTiming();
        auto dist = distanceToSquare(xVal, yVal, centreX, centreY, width, height);
        benchmark::DoNotOptimize(dist);
    }
}

BENCHMARK(distToSquareNative);
