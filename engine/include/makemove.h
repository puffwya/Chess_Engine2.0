#pragma once

#include "position.h"
#include "move.h"

struct UndoInfo
{
    uint8_t capturedPiece;   // 0 if none
    bool    castlingRights;  // (expand later)
    bool    wasWhite;
    int     enPassantSquare;
};

class MoveMaker
{
public:
    static void makeMove(Position& pos, const Move& move, UndoInfo& undo);
    static void unmakeMove(Position& pos, const Move& move, const UndoInfo& undo);
};
