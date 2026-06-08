#include "search.h"
#include "movegen.h"
#include "makemove.h"
#include "evaluate.h"

#include <vector>
#include <algorithm>

static constexpr int INF = 100000;

namespace
{
    constexpr int MAX_DEPTH = 64;

    // -------------------------
    // Killer moves (2 per depth)
    // -------------------------
    Move killer1[MAX_DEPTH];
    Move killer2[MAX_DEPTH];

    // -------------------------
    // History heuristic table
    // -------------------------
    int history[64][64] = {0};

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

    static int scoreMove(const Position& pos, const Move& m, int depth)
    {
        int score = 0;

        if (m.flags & CAPTURE)
        {
            int victim   = pieceValueAt(pos, m.to);
            int attacker = pieceValueAt(pos, m.from);
            score += 10000 + victim - attacker;
        }

        if (m.flags & PROMOTION)
            score += 20000;

        // killer move bonus
        if (killer1[depth].from == m.from && killer1[depth].to == m.to)
            score += 9000;

        if (killer2[depth].from == m.from && killer2[depth].to == m.to)
            score += 8000;

        // history heuristic
        score += history[m.from][m.to];

        return score;
    }

    int negamax(Position& pos, int depth, int alpha, int beta, int ply)
    {
        if (depth == 0)
            return Evaluate::evaluate(pos);

        std::vector<Move> moves;
        MoveGenerator::generateMoves(pos, moves);

        if (moves.empty())
            return Evaluate::evaluate(pos);

        std::sort(moves.begin(), moves.end(),
            [&](const Move& a, const Move& b)
            {
                return scoreMove(pos, a, ply) > scoreMove(pos, b, ply);
            });

        int best = -INF;

        for (const Move& m : moves)
        {
            Position copy = pos;
            UndoInfo undo;

            MoveMaker::makeMove(copy, m, undo);

            int score = -negamax(copy, depth - 1, -beta, -alpha, ply + 1);

            if (score > best)
                best = score;

            if (score > alpha)
            {
                alpha = score;

                // update history
                history[m.from][m.to] += depth * depth;
            }

            if (alpha >= beta)
            {
                // update killer moves (quiet moves only)
                if (!(m.flags & CAPTURE))
                {
                    if (!(killer1[ply].from == m.from && killer1[ply].to == m.to))
                    {
                        killer2[ply] = killer1[ply];
                        killer1[ply] = m;
                    }
                }
                break;
            }
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

        std::sort(moves.begin(), moves.end(),
            [&](const Move& a, const Move& b)
            {
                return scoreMove(pos, a, 0) > scoreMove(pos, b, 0);
            });

        Move bestMove = moves[0];
        int bestScore = -INF;

        for (const Move& m : moves)
        {
            Position copy = pos;
            UndoInfo undo;

            MoveMaker::makeMove(copy, m, undo);

            int score = -negamax(copy, depth - 1, -INF, INF, 0);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = m;
            }
        }

        return bestMove;
    }
}
