#include "movegen.h"
#include "movegen/pawn.cpp"
#include "movegen/knight.cpp"

void generateWhitePawnMoves(const Position&, std::vector<Move>&);
void generateBlackPawnMoves(const Position&, std::vector<Move>&);

void generateWhiteKnightMoves(const Position&, std::vector<Move>&);
void generateBlackKnightMoves(const Position&, std::vector<Move>&);

void generateWhiteBishopMoves(const Position&, std::vector<Move>&);
void generateBlackBishopMoves(const Position&, std::vector<Move>&);

void generateWhiteRookMoves(const Position&, std::vector<Move>&);
void generateBlackRookMoves(const Position&, std::vector<Move>&);

void generateWhiteQueenMoves(const Position&, std::vector<Move>&);
void generateBlackQueenMoves(const Position&, std::vector<Move>&);

void generateWhiteKingMoves(const Position&, std::vector<Move>&);
void generateBlackKingMoves(const Position&, std::vector<Move>&);

void MoveGenerator::generateMoves(const Position& pos, std::vector<Move>& moves)
{
    moves.clear();

    if (pos.sideToMove == WHITE)
    {
        generateWhitePawnMoves(pos, moves);
        generateWhiteKnightMoves(pos, moves);
        generateWhiteBishopMoves(pos, moves);
        generateWhiteRookMoves(pos, moves);
        generateWhiteQueenMoves(pos, moves);
        generateWhiteKingMoves(pos, moves);
    }
    else
    {
        generateBlackPawnMoves(pos, moves);
        generateBlackKnightMoves(pos, moves);
        generateBlackBishopMoves(pos, moves);
        generateBlackRookMoves(pos, moves);
        generateBlackQueenMoves(pos, moves);
        generateBlackKingMoves(pos, moves);
    }
}
