cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
project(ChessEngine)

enable_testing()

add_compile_options(-g)
add_library(
    engine
    src/bitboard.cpp
    src/board.cpp
    src/movegen.cpp
    src/moves.cpp
)

target_include_directories(engine PUBLIC include)

add_subdirectory(tests)
