project(Tests)

enable_testing()
find_package(GTest)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            Symbolic/CompiletimeConstant.cpp
            Symbolic/LongVariableNames.cpp
            Symbolic/Operations/Add.cpp
            Symbolic/Operations/Div.cpp
            Symbolic/Operations/Functions/ArcCos.cpp
            Symbolic/Operations/Functions/ArcSin.cpp
            Symbolic/Operations/Functions/ArcTan.cpp
            Symbolic/Operations/Functions/Cos.cpp
            Symbolic/Operations/Functions/Exp.cpp
            Symbolic/Operations/Functions/Log.cpp
            Symbolic/Operations/Functions/Pow.cpp
            Symbolic/Operations/Functions/Sin.cpp
            Symbolic/Operations/Functions/Sqrt.cpp
            Symbolic/Operations/Functions/Tan.cpp
            Symbolic/Operations/Greater.cpp
            Symbolic/Operations/Mul.cpp
            Symbolic/Operations/Not.cpp
            Symbolic/Operations/Sub.cpp
            Symbolic/Operations/Ternary.cpp
            Symbolic/Operators.cpp
            Symbolic/RuntimeConstant.cpp
            Symbolic/Simplifier/CompileTime.cpp
            Symbolic/Simplifier/ExpressionTraverse.cpp
            Symbolic/Simplifier/Runtime.cpp
            Symbolic/Simplifier/Simplifier.cpp
            Symbolic/Sum.cpp
            Symbolic/ToString.cpp
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
