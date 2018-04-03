#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <iostream>
#include <vector>

#include "settings.h"
#include "orb.h"
#include "combo.h"



class Board
{

public:
    orb orbs[BOARD_X][BOARD_Y];

    Board(std::string orbcode = "");

    // orb loading
    void loadRandom();
    void loadOrbcode(std::string orbcode);

    // orb movement
    void swapOrbs(int x1, int y1, int x2, int y2);

    // comboing
    bool hasMatches();
    bool removeComboOne();
    void removeComboAll();
    bool dropDownOne();
    void dropDownAll();

    // getting combos
    std::vector<std::vector<Combo>> getCombos();

    // filling
    void fillEmpty();

    // score calculation
    int    combos   = 0;
    int    cascades = 0;
    double score    = 0;
    void   evaluateBoard();

    // setting orb locations
    void setStartLocations();
    void setFinishLocations();

private:
    // evaluated
    bool evaluated;

    // a board containing numbers corresponding to combos
    int comboBoard[BOARD_X][BOARD_Y];

    // a board containing orbs that are part of a match
    orb matchBoard[BOARD_X][BOARD_Y];

    // a list of a list of combos at each cascade level
    std::vector<std::vector<Combo>> comboList;

    // these functions analyze the board
    // but should never change the state of the array orbs
    int  findCombos();
    bool filterMatches();
    bool findFirstNonempty(int *x, int *y);
    void floodFill(int x, int y, int fillNumber);
    void floodFillRecursive(int x, int y, int fillNumber, int orbColor, bool visited[BOARD_X][BOARD_Y]);

};

// print board
std::ostream &operator<<(std::ostream &os, Board const &b);


#endif // BOARD_H
