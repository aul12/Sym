cmake_minimum_required(VERSION 3.14)
project(Sym++)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

option(BUILD_TESTS "Build Tests" OFF)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")
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
        Source/Symbolic/Operations/Functions/Cos.hpp
        Source/Symbolic/Operations/Functions/Exp.hpp
        Source/Symbolic/Operations/Functions/Functions.hpp
        Source/Symbolic/Operations/Functions/Sin.hpp
        Source/Symbolic/Operations/Functions/Sqrt.hpp
        Source/Symbolic/Operations/Functions/Tan.hpp
        Source/Symbolic/Operations/Greater.hpp
        Source/Symbolic/Operations/Mul.hpp
        Source/Symbolic/Operations/Sub.hpp
        Source/Symbolic/Operations/Ternary.hpp
        Source/Symbolic/Operators.hpp
        Source/Symbolic/RuntimeConstant.hpp
        Source/Symbolic/Sum.hpp
        Source/Symbolic/Variable.hpp
        Source/Symbolic/Vector.hpp
        )

set(LIBS)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC Source)

#target_link_libraries(${PROJECT_NAME} ${LIBS})

if (BUILD_TESTS)
    add_subdirectory(Test)
endif ()
