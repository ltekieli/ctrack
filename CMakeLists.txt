cmake_minimum_required(VERSION 3.12)

project(ctrack
    LANGUAGES CXX
    VERSION 0.1
)

option(BUILD_TESTS "Enable building test targets" OFF)

add_subdirectory(lib)
add_subdirectory(bin)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
