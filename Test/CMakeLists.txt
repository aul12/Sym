project(Tests)

enable_testing()
find_package(GTest)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            Symbolic/CompiletimeConstant.cpp
            Symbolic/Operations/Add.cpp
            Symbolic/Operations/Div.cpp
            Symbolic/Operations/Functions/Cos.cpp
            Symbolic/Operations/Functions/Exp.cpp
            Symbolic/Operations/Functions/Sin.cpp
            Symbolic/Operations/Functions/Sqrt.cpp
            Symbolic/Operations/Functions/Tan.cpp
            Symbolic/Operations/Mul.cpp
            Symbolic/Operations/Ternary.cpp
            Symbolic/Operators.cpp
            Symbolic/RuntimeConstant.cpp
            Symbolic/Sum.cpp
            Symbolic/Variable.cpp
            Symbolic/Vector.cpp
            )

    find_package (Eigen3 3.3 REQUIRED NO_MODULE)

    add_executable(${PROJECT_NAME} main.cpp ${TEST_SOURCES})

    target_link_libraries(${PROJECT_NAME} Sym++ gmock gtest pthread Eigen3::Eigen)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
    )
endif ()
