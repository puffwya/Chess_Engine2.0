cmake_minimum_required(VERSION 3.16)
project(Chess_Engine2_0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(engine/include)

add_library(engine
    engine/src/position.cpp
    engine/src/move.cpp
    engine/src/movegen.cpp
    engine/src/attacks.cpp
    engine/src/bitboard.cpp
    engine/src/sliders.cpp
    engine/src/legality.cpp
    engine/src/movegen/bishop.cpp
    engine/src/movegen/pawn.cpp
    engine/src/movegen/knight.cpp
    engine/src/movegen/rook.cpp
    engine/src/movegen/queen.cpp
    engine/src/movegen/king.cpp
)

add_executable(perft tests/perft.cpp)

target_link_libraries(perft engine)
