#include "search.h"
#include "movegen.h"
#include "makemove.h"
#include "evaluate.h"

#include <vector>
#include <limits>
#include <algorithm>

static constexpr int INF = 100000;

namespace
{
    static int pieceValueAt(const Position& pos, int sq)
    {
        Bitboard bb = 1ULL << sq;

        if ((pos.whitePawns   | pos.blackPawns)   & bb) return 100;
        if ((pos.whiteKnights | pos.blackKnights) & bb) return 320;
        if ((pos.whiteBishops | pos.blackBishops) & bb) return 330;
        if ((pos.whiteRooks   | pos.blackRooks)   & bb) return 500;
        if ((pos.whiteQueens  | pos.blackQueens)  & bb) return 900;
        if ((pos.whiteKing    | pos.blackKing)    & bb) return 20000;

        return 0;
    }

    static int scoreMove(const Position& pos, const Move& m)
    {
        int score = 0;

        // Promotions first
        if (m.flags & PROMOTION)
        {
            score += 100000;

            switch (m.promo)
            {
                case QUEEN:  score += 900; break;
                case ROOK:   score += 500; break;
                case BISHOP: score += 330; break;
                case KNIGHT: score += 320; break;
                default: break;
            }
        }

        // MVV-LVA capture ordering
        if (m.flags & CAPTURE)
        {
            int victim   = pieceValueAt(pos, m.to);
            int attacker = pieceValueAt(pos, m.from);

            score += 10000 + victim - attacker;
        }

        return score;
    }

    int negamax(Position& pos, int depth, int alpha, int beta)
    {
        if (depth == 0)
            return Evaluate::evaluate(pos);

        std::vector<Move> moves;
        MoveGenerator::generateMoves(pos, moves);

        if (moves.empty())
            return Evaluate::evaluate(pos);

        std::sort(
            moves.begin(),
            moves.end(),
            [&](const Move& a, const Move& b)
            {
                return scoreMove(pos, a) > scoreMove(pos, b);
            });

        int best = -INF;

        for (const Move& m : moves)
        {
            Position copy = pos;
            UndoInfo undo;

            MoveMaker::makeMove(copy, m, undo);

            int score = -negamax(copy, depth - 1, -beta, -alpha);

            if (score > best)
                best = score;

            if (score > alpha)
                alpha = score;

            if (alpha >= beta)
                break;
        }

        return best;
    }
}

namespace Search
{
    Move findBestMove(Position& pos, int depth)
    {
        std::vector<Move> moves;
        MoveGenerator::generateMoves(pos, moves);

        std::sort(
            moves.begin(),
            moves.end(),
            [&](const Move& a, const Move& b)
            {
                return scoreMove(pos, a) > scoreMove(pos, b);
            });

        Move bestMove = moves[0];
        int bestScore = -INF;

        for (const Move& m : moves)
        {
            Position copy = pos;
            UndoInfo undo;

            MoveMaker::makeMove(copy, m, undo);

            int score = -negamax(copy, depth - 1, -INF, INF);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = m;
            }
        }

        return bestMove;
    }
}
