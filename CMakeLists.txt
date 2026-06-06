cmake_minimum_required(VERSION 3.16)
project(Chess_Engine2_0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(engine/include)

# -------------------------
# ENGINE (native)
# -------------------------
add_library(engine
    engine/src/position.cpp
    engine/src/move.cpp
    engine/src/movegen.cpp
    engine/src/attacks.cpp
    engine/src/bitboard.cpp
    engine/src/sliders.cpp
    engine/src/legality.cpp
    engine/src/position_init.cpp
    engine/src/perft.cpp
    engine/src/makemove.cpp
    engine/src/attack.cpp
    engine/src/movegen/bishop.cpp
    engine/src/movegen/pawn.cpp
    engine/src/movegen/knight.cpp
    engine/src/movegen/rook.cpp
    engine/src/movegen/queen.cpp
    engine/src/movegen/king.cpp
)

add_executable(perft tests/perft.cpp)
target_link_libraries(perft engine)

# -------------------------
# WASM BUILD (separate target)
# -------------------------

file(COPY ${CMAKE_SOURCE_DIR}/web/
     DESTINATION ${CMAKE_BINARY_DIR}/web)

add_executable(chess_wasm
    engine/src/wasm_bindings.cpp
    engine/src/movegen.cpp
    engine/src/makemove.cpp
    engine/src/position.cpp
    engine/src/position_init.cpp
    engine/src/bitboard.cpp
    engine/src/attacks.cpp
    engine/src/sliders.cpp

    engine/src/movegen/pawn.cpp
    engine/src/movegen/knight.cpp
    engine/src/movegen/bishop.cpp
    engine/src/movegen/rook.cpp
    engine/src/movegen/queen.cpp
    engine/src/movegen/king.cpp
)

set_target_properties(chess_wasm PROPERTIES
    OUTPUT_NAME "chess_wasm"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/web"
)

target_link_options(chess_wasm PRIVATE
    -O3
    -sWASM=1
    -sMODULARIZE=1
    -sEXPORT_NAME=createModule
    -sENVIRONMENT=web

    -sEXPORTED_FUNCTIONS=_initPosition,_getPiece,_debugPrintBoard

    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
)
