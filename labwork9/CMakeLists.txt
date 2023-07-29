cmake_minimum_required(VERSION 3.12)

project(
        labwork9
        VERSION 1.0
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23) # use 17

add_subdirectory(lib)
add_subdirectory(bin)