#include "position.h"
#include "position_init.h"
#include "perft.h"
#include <iostream>

int main()
{
    Position pos = PositionInit::startpos();

    std::cout << "Perft divide depth 2:\n";

    Perft::perftDivide(pos, 2);

    return 0;
}
