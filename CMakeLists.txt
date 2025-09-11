cmake_minimum_required(VERSION 3.14)
project(breakout C CXX)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

add_executable(breakout src/main.cpp)
target_link_libraries(breakout raylib)
