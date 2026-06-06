#include <emscripten/emscripten.h>
#include <vector>

#include "position.h"
#include "position_init.h"
#include "movegen.h"
#include "attacks.h"
#include "makemove.h"
#include "bitboard.h"

// -------------------------
// GLOBAL STATE (debug engine only)
// -------------------------
static Position g_pos;
static std::vector<Move> g_moves;
static UndoInfo g_undo;

// -------------------------
// INIT
// -------------------------
extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void initPosition()
    {
        g_pos = PositionInit::startpos();
        g_moves.clear();
        initKnightAttacks();
        initKingAttacks();
    }

    // -------------------------
    // BOARD QUERY
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    int getPiece(int sq)
    {
        Bitboard bb = 1ULL << sq;

        if (g_pos.whitePawns & bb) return 1;
        if (g_pos.whiteKnights & bb) return 2;
        if (g_pos.whiteBishops & bb) return 3;
        if (g_pos.whiteRooks & bb) return 4;
        if (g_pos.whiteQueens & bb) return 5;
        if (g_pos.whiteKing & bb) return 6;

        if (g_pos.blackPawns & bb) return -1;
        if (g_pos.blackKnights & bb) return -2;
        if (g_pos.blackBishops & bb) return -3;
        if (g_pos.blackRooks & bb) return -4;
        if (g_pos.blackQueens & bb) return -5;
        if (g_pos.blackKing & bb) return -6;

        return 0;
    }

    // -------------------------
    // MOVE GENERATION (ONE SQUARE CONTEXT)
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    int selectSquare(int sq)
    {
        g_moves.clear();
        MoveGenerator::generateMoves(g_pos, g_moves);

        int count = 0;

        for (const Move& m : g_moves)
            if (m.from == sq)
                count++;

        return count;
    }

    EMSCRIPTEN_KEEPALIVE
    int getMoveTo(int sq, int index)
    {
        int count = 0;

        for (const Move& m : g_moves)
        {
            if (m.from != sq) continue;

            if (count == index)
                return m.to;

            count++;
        }

        return -1;
    }

    // -------------------------
    // MAKE MOVE (SAFE)
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    void makeMove(int from, int to)
    {
        for (const Move& m : g_moves)
        {
            if (m.from == from && m.to == to)
            {
                MoveMaker::makeMove(g_pos, m, g_undo);
                return;
            }
        }
    }

    // -------------------------
    // DEBUG
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    void debugPrintBoard()
    {
        int whitePawns = __builtin_popcountll(g_pos.whitePawns);
        printf("White pawns: %d\n", whitePawns);
    }
}
