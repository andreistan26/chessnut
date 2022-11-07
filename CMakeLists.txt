cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ChessEngine)

enable_testing()
add_compile_options(-g -Wall -std=c++2a -mavx2 -pg -O3)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
add_library(
    libs
    src/bitboard.cpp
    src/board.cpp
    src/movegen.cpp
    src/moves.cpp
    src/communication.cpp
    src/engine.cpp
    src/evaluate.cpp
    src/undo_stack.cpp
    src/search.cpp
    src/logger.cpp
)

target_include_directories(libs PUBLIC include)

#test
add_subdirectory(tests)

#engine main
add_executable(engine src/main.cpp)
target_link_libraries(engine libs)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../..)

