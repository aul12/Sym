#include <Symbolic/Vector.hpp>
#include <benchmark/benchmark.h>

#include "Symbolic/Operations/Functions/Functions.hpp"
#include "Symbolic/Operators.hpp"
#include "Symbolic/Simplifier/Simplifier.hpp"

#include <iostream>

auto makeVec = [](auto &&...args) { return sym::Vector{args...}; };

auto makeArr = [](auto &&...args) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
    return std::array<decltype((... + args)), sizeof...(args)>{args...};
#pragma GCC diagnostic pop
};

template<typename F, typename XPos, typename YPos, typename Vel, typename Psi, typename Acc, typename Steer>
auto f(F factory, XPos xPos, YPos yPos, Vel vel, Psi psi, Acc acc, Steer steer, double dt) {
    // clang-format off
    return factory(
        xPos + std::cos(psi) * vel * dt,
        yPos + std::sin(psi) * vel * dt,
        vel + acc * dt,
        psi + vel / 0.5 * std::sin(steer) * dt
    );
    // clang-format on
}

template<typename F, typename XPos, typename YPos, typename Vel, typename Psi, typename Acc, typename Steer>
auto dx_f(F factory, XPos /*xPos*/, YPos /*yPos*/, Vel vel, Psi psi, Acc /*acc*/, Steer steer, double dt) {
    // clang-format off
    return factory(
        1, 0, std::cos(psi) * dt, -std::sin(psi) * vel * dt,
        0, 1, std::sin(psi) * dt, std::cos(psi) * vel * dt,
        0, 0, 1, 0,
        0, 1/0.5 * std::sin(steer) * dt, vel / 0.5 * dt * std::cos(steer)
    );
    // clang-format on
}

template<typename F, typename XPos, typename YPos, typename Vel, typename Psi, typename Acc, typename Steer>
auto du_f(F factory, XPos /*xPos*/, YPos /*yPos*/, Vel vel, Psi /*psi*/, Acc /*acc*/, Steer steer, double dt) {
    // clang-format off
    return factory(
            0, 0,
            0, 0,
            dt, 0,
            0, vel/0.5 * std::cos(steer) * dt
    );
    // clang-format on
}

void gradientResolveSym(benchmark::State &state) {
    sym::Variable<'x'> xPos;
    sym::Variable<'y'> yPos;
    sym::Variable<'v'> vel;
    sym::Variable<'p'> psi;

    sym::Variable<'a'> acc;
    sym::Variable<'s'> steer;

    sym::Vector x{xPos, yPos, vel, psi};
    sym::Vector u{acc, steer};

    auto func = f(makeVec, xPos, yPos, vel, psi, acc, steer, 0.1);
    auto dx_func = sym::gradient(func, x);
    auto du_func = sym::gradient(func, u);

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double velVal = rand();
        double psiVal = rand();
        double accVal = rand();
        double steerVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(
                func.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal, acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(
                dx_func.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal, acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(
                du_func.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal, acc = accVal, steer = steerVal));
    }
}

BENCHMARK(gradientResolveSym);

void gradientResolveSymSimplified(benchmark::State &state) {
    sym::Variable<'x'> xPos;
    sym::Variable<'y'> yPos;
    sym::Variable<'v'> vel;
    sym::Variable<'p'> psi;

    sym::Variable<'a'> acc;
    sym::Variable<'s'> steer;

    sym::Vector x{xPos, yPos, vel, psi};
    sym::Vector u{acc, steer};

    auto func = f(makeVec, xPos, yPos, vel, psi, acc, steer, 0.1);
    auto dx_func = sym::gradient(func, x);
    auto du_func = sym::gradient(func, u);
    auto func_simplified = sym::simplifier::simplify(func);
    auto dx_func_simplified = sym::simplifier::simplify(dx_func);
    auto du_func_simplified = sym::simplifier::simplify(du_func);

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double velVal = rand();
        double psiVal = rand();
        double accVal = rand();
        double steerVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                         acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(dx_func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                            acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(du_func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                            acc = accVal, steer = steerVal));
    }
}

BENCHMARK(gradientResolveSymSimplified);

void gradientResolveSymSimplifiedWithPrint(benchmark::State &state) {
    sym::Variable<'x'> xPos;
    sym::Variable<'y'> yPos;
    sym::Variable<'v'> vel;
    sym::Variable<'p'> psi;

    sym::Variable<'a'> acc;
    sym::Variable<'s'> steer;

    sym::Vector x{xPos, yPos, vel, psi};
    sym::Vector u{acc, steer};

    auto func = f(makeVec, xPos, yPos, vel, psi, acc, steer, 0.1);
    auto dx_func = sym::gradient(func, x);
    auto du_func = sym::gradient(func, u);
    auto func_simplified = sym::simplifier::simplify(func);
    auto dx_func_simplified = sym::simplifier::simplify(dx_func);
    auto du_func_simplified = sym::simplifier::simplify(du_func);

    std::cout << sym::toString(dx_func_simplified) << "\n\n";

    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double velVal = rand();
        double psiVal = rand();
        double accVal = rand();
        double steerVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                         acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(dx_func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                            acc = accVal, steer = steerVal));
        benchmark::DoNotOptimize(du_func_simplified.resolve(xPos = xVal, yPos = yVal, vel = velVal, psi = psiVal,
                                                            acc = accVal, steer = steerVal));
    }
}

BENCHMARK(gradientResolveSymSimplifiedWithPrint);

void gradientResolveNative(benchmark::State &state) {
    for (auto _ : state) {
        state.PauseTiming();
        double xVal = rand();
        double yVal = rand();
        double velVal = rand();
        double psiVal = rand();
        double accVal = rand();
        double steerVal = rand();
        state.ResumeTiming();
        benchmark::DoNotOptimize(f(makeArr, xVal, yVal, velVal, psiVal, accVal, steerVal, 0.1));
        benchmark::DoNotOptimize(dx_f(makeArr, xVal, yVal, velVal, psiVal, accVal, steerVal, 0.1));
        benchmark::DoNotOptimize(du_f(makeArr, xVal, yVal, velVal, psiVal, accVal, steerVal, 0.1));
    }
}

BENCHMARK(gradientResolveNative);
