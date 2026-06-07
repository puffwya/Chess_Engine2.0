#include "search.h"
#include "movegen.h"
#include "makemove.h"
#include "evaluate.h"

#include <vector>
#include <limits>

static constexpr int INF = 100000;

namespace
{
    int negamax(Position& pos, int depth, int alpha, int beta)
    {
        if (depth == 0)
            return Evaluate::evaluate(pos);

        std::vector<Move> moves;
        MoveGenerator::generateMoves(pos, moves);

        if (moves.empty())
            return Evaluate::evaluate(pos);

        int best = -INF;

        for (const Move& m : moves)
        {
            Position copy = pos;
            UndoInfo undo;

            MoveMaker::makeMove(copy, m, undo);

            // KEY IDEA: negate score instead of tracking side manually
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
