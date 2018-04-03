#ifndef SOLVESTATE_H
#define SOLVESTATE_H

#include <deque>
#include <vector>
#include <iostream>

#include "board.h"


enum movetype {
    RIGHT,
    UP,
    LEFT,
    DOWN
};


class SolveState
{
public:
    Board board;
    int fingerX;
    int fingerY;
    int startX;
    int startY;
    double score;
    std::vector<int> moves;
    std::vector<Combo> combos;

    SolveState();
    SolveState(Board b, int x, int y);

    void addNewStates(std::vector<SolveState> &stateList);


private:
    void calculateScore();

};

// sorting greatest to least
// the "less than" operator is intentionally defined to be greater than
bool operator<(SolveState const& a, SolveState const& b);

// printing
std::ostream &operator<<(std::ostream &os, SolveState & s);

#endif // SOLVESTATE_H
