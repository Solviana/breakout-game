cmake_minimum_required(VERSION 3.16)


set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Wall -O3" CACHE STRING "C++ flag for debug build" FORCE)
set(CMAKE_CXX_CLANG_TIDY
    clang-tidy
    -checks=*,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-fuchsia-default-arguments-calls,-fuchsia-overloaded-operator)

project(breakout)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
enable_testing()

find_package(
    SFML
    COMPONENTS graphics window
)

find_package(
    GTest
)

add_subdirectory(src)
add_subdirectory(test)
