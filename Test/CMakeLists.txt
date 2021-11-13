project(Tests)

enable_testing()
find_package(GTest)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            Symbolic/Constant.cpp
            Symbolic/Operations/Add.cpp
            Symbolic/Operations/Div.cpp
            Symbolic/Operations/Functions/Cos.cpp
            Symbolic/Operations/Functions/Sin.cpp
            Symbolic/Operations/Functions/Tan.cpp
            Symbolic/Operations/Mul.cpp
            Symbolic/Operators.cpp
            Symbolic/Sum.cpp
            Symbolic/Variable.cpp
            )

    add_executable(${PROJECT_NAME} main.cpp ${TEST_SOURCES})

    target_link_libraries(${PROJECT_NAME} Sym++ gmock gtest pthread)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
    )
endif ()
