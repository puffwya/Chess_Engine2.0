#pragma once

#include "position.h"
#include "move.h"

namespace Search
{
    Move findBestMove(Position& pos, int depth);
}
