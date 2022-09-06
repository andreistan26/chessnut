cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ChessEngine)

#enable_testing()
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
add_compile_options(-g -Wall)
add_library(
    libs
    src/bitboard.cpp
    src/board.cpp
    src/movegen.cpp
    src/moves.cpp
    src/communication.cpp
    src/engine.cpp
)

target_include_directories(libs PUBLIC include)
add_executable(engine src/main.cpp)
target_link_libraries(engine libs)
#add_subdirectory(tests)
