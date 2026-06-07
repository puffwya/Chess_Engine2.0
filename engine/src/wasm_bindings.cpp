#include <emscripten/emscripten.h>
#include <vector>

#include "position.h"
#include "position_init.h"
#include "movegen.h"
#include "evaluate.h"
#include "search.h"
#include "attack.h"
#include "attacks.h"
#include "makemove.h"
#include "bitboard.h"

// -------------------------
// GLOBAL STATE
// -------------------------
static Position g_pos;
static std::vector<Move> g_moves;
static std::vector<Move> g_selectedMoves;
static UndoInfo g_undo;

extern "C" void generateLegalMoves();

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
        g_selectedMoves.clear();

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
    // MOVE GENERATION (SELECTED PIECE)
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    int selectSquare(int sq)
    {
        generateLegalMoves();

        g_selectedMoves.clear();

        for (const Move& m : g_moves)
        {
            if (m.from == sq)
                g_selectedMoves.push_back(m);
        }

        return (int)g_selectedMoves.size();
    }

    EMSCRIPTEN_KEEPALIVE
    int getSelectedMoveTo(int index)
    {
        return g_selectedMoves[index].to;
    }

    EMSCRIPTEN_KEEPALIVE
    int getSelectedMoveFlags(int index)
    {
        return g_selectedMoves[index].flags;
    }

    EMSCRIPTEN_KEEPALIVE
    int getSelectedMovePromo(int index)
    {
        return g_selectedMoves[index].promo;
    }

    EMSCRIPTEN_KEEPALIVE
    void makeAIMove()
    {
        generateLegalMoves();

        if (g_moves.empty())
            return;

        Move best = Search::findBestMove(g_pos, 3); // start with depth 3

        MoveMaker::makeMove(g_pos, best, g_undo);

        g_moves.clear();
    }

    // -------------------------
    // IMPORTANT: DEBUG/LEGACY ACCESS
    // -------------------------
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

    EMSCRIPTEN_KEEPALIVE
    int getMovePromo(int index)
    {
        return g_selectedMoves[index].promo;
    }

    EMSCRIPTEN_KEEPALIVE
    int getMoveFlags(int index)
    {
        return g_selectedMoves[index].flags;
    }

    // -------------------------
    // MAKE MOVE
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    bool makeMove(int from, int to)
    {
        for (const Move& m : g_moves)
        {
            if (m.from == from && m.to == to)
            {
                MoveMaker::makeMove(g_pos, m, g_undo);

                g_moves.clear();
                MoveGenerator::generateMoves(g_pos, g_moves);

                return true;
            }
        }

        return false;
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

    EMSCRIPTEN_KEEPALIVE
    int evaluatePosition()
    {
        return Evaluate::evaluate(g_pos);
    }

    // -------------------------
    // LEGAL MOVE FILTERING
    // -------------------------
    EMSCRIPTEN_KEEPALIVE
    void generateLegalMoves()
    {
        g_moves.clear();

        MoveGenerator::generateMoves(g_pos, g_moves);

        std::vector<Move> legal;
        legal.reserve(g_moves.size());

        for (const Move& m : g_moves)
        {
            Position copy = g_pos;
            UndoInfo undo;

            Color us = copy.sideToMove;

            MoveMaker::makeMove(copy, m, undo);

            int kingSq = (us == WHITE)
                ? bb::lsb(copy.whiteKing)
                : bb::lsb(copy.blackKing);

            Color them = (us == WHITE ? BLACK : WHITE);

            bool inCheck = Attack::isSquareAttacked(copy, kingSq, them);

            if (!inCheck)
                legal.push_back(m);
        }

        g_moves = std::move(legal);
    }
}
