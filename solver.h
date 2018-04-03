#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "board.h"
#include "solvestate.h"


SolveState solveBoard(Board b);
std::vector<SolveState> solveBoardAll(Board b);

#endif // SOLVER_H
