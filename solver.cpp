#include <iterator>
#include <algorithm>
#include <iostream>

#include "settings.h"
#include "solver.h"

SolveState solveBoard(Board b)
{
    return solveBoardAll(b).front();
}

std::vector<SolveState> solveBoardAll(Board b)
{
    int i, j;

    std::vector<SolveState> solutions;

    b.setStartLocations();

    // initialize starting locations
    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            solutions.push_back(SolveState(b, i, j));
    std::sort(solutions.begin(), solutions.end());

    // go through all move lengths
    for(unsigned int n_moves = 0; n_moves < MAX_MOVES; n_moves++) {
        std::vector<SolveState> newSolutions;
        // make a move on all states with the current highest move length
        for(auto &iter : solutions)
            if(iter.moves.size() == n_moves)
                iter.addNewStates(newSolutions);

        // add new states to solutions list
        for(auto &iter : newSolutions)
            solutions.push_back(iter);

        // sort all solutions
        std::sort(solutions.begin(), solutions.end());

        // remove excess solutions
        while(solutions.size() > NUM_SOLUTIONS) {
            solutions.pop_back();
        }
    }

    return solutions;
}
