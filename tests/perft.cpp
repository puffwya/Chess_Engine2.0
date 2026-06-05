#include <iostream>
#include <vector>
#include "position.h"
#include "movegen.h"

int main()
{
    Position pos{};

    pos.whitePawns = 0x000000000000FF00ULL;
    pos.blackPawns = 0x00FF000000000000ULL;
    pos.sideToMove = WHITE;

    std::vector<Move> moves;

    MoveGenerator::generateMoves(pos, moves);

    std::cout << "Moves: " << moves.size() << "\n";

    for (auto& m : moves)
        std::cout << (int)m.from << " -> " << (int)m.to << "\n";
}
