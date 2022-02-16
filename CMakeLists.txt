cmake_minimum_required(VERSION 3.14)
project(Sym++)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

option(BUILD_TESTS "Build Tests" OFF)
option(SIMPLIFY_GRADIENTS "Simplify the gradients during calculation" OFF)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native -g")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -DDEBUG")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif ()

set(SOURCES
        Source/Symbolic/CompiletimeConstant.hpp
        Source/Symbolic/Expression.hpp
        Source/Symbolic/Operations/Add.hpp
        Source/Symbolic/Operations/Div.hpp
        Source/Symbolic/Operations/Functions/ArcCos.hpp
        Source/Symbolic/Operations/Functions/ArcSin.hpp
        Source/Symbolic/Operations/Functions/ArcTan.hpp
        Source/Symbolic/Operations/Functions/ArcTan2.hpp
        Source/Symbolic/Operations/Functions/Cos.hpp
        Source/Symbolic/Operations/Functions/Exp.hpp
        Source/Symbolic/Operations/Functions/Functions.hpp
        Source/Symbolic/Operations/Functions/Log.hpp
        Source/Symbolic/Operations/Functions/Pow.hpp
        Source/Symbolic/Operations/Functions/Sin.hpp
        Source/Symbolic/Operations/Functions/Sqrt.hpp
        Source/Symbolic/Operations/Functions/Tan.hpp
        Source/Symbolic/Operations/Greater.hpp
        Source/Symbolic/Operations/Mul.hpp
        Source/Symbolic/Operations/Not.hpp
        Source/Symbolic/Operations/Sub.hpp
        Source/Symbolic/Operations/Ternary.hpp
        Source/Symbolic/Operators.hpp
        Source/Symbolic/RuntimeConstant.hpp
        Source/Symbolic/Simplifier/CompileTime.hpp
        Source/Symbolic/Simplifier/ExpressionTraverse.hpp
        Source/Symbolic/Simplifier/Runtime.hpp
        Source/Symbolic/Simplifier/Simplifier.hpp
        Source/Symbolic/Sum.hpp
        Source/Symbolic/Variable.hpp
        Source/Symbolic/Vector.hpp
        )

set(LIBS)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC Source)

if (${SIMPLIFY_GRADIENTS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC _GRADIENT_SIMPLIFICATION)
    message(STATUS "Gradient simplification")
else ()
    message(STATUS "No Gradient simplification")
endif ()

#target_link_libraries(${PROJECT_NAME} ${LIBS})

include(GNUInstallDirs)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${SOURCES}")
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if (BUILD_TESTS)
    add_subdirectory(Test)
    add_subdirectory(Benchmark)
endif ()
