find_package(GTest)

if (GTest_FOUND)
    find_package(benchmark)
    if (benchmark_FOUND)
        project(Benchmark)

        set(SOURCES
                Symbolic/ComplexResolve.cpp
               # Symbolic/EllipseDistance.cpp
                Symbolic/ManyVariables.cpp
                Symbolic/GradientResolve.cpp)

        add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
        target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark pthread Sym++)
        set_property(TARGET ${PROJECT_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
    else ()
        message(WARNING "benchmark not found")
    endif ()
else ()
    message(WARNING "GTest required for benchmarks")
endif ()
