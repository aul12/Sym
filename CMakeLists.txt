cmake_minimum_required(VERSION 3.14)
project(Symbolic++)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

option(BUILD_TESTS "Build Tests" ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -DDEBUG")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif ()

set(SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Constant.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Expression.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Add.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Div.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Functions/Cos.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Functions/Functions.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Functions/Sin.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Functions/Tan.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Mul.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operations/Sub.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Operators.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Sum.hpp
        ${CMAKE_SOURCE_DIR}/Source/Symbolic/Variable.hpp
        )

set(LIBS)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Source)

#target_link_libraries(${PROJECT_NAME} ${LIBS})

if (BUILD_TESTS)
    add_subdirectory(Test)
endif ()
