#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

#include "board.h"

Board::Board(std::string orbcode)
{
    loadRandom();

    if(orbcode.length() >= BOARD_X * BOARD_Y)
        loadOrbcode(orbcode);
}

void Board::loadRandom()
{
    int i, j;

    // fill randomly
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            orbs[i][j].color = makeRandomOrb();
        }
    }

    // clear out combos
    while(hasMatches()) {
        removeComboAll();
        dropDownAll();
        fillEmpty();
    }

    setStartLocations();
}

void Board::loadOrbcode(std::string orbcode)
{
    int i, j;
    char c;

    if(orbcode.length() < BOARD_X * BOARD_Y) {
        std::cerr << "Error loading orb code: too short" << std::endl;
        return;
    }

    auto iter = orbcode.begin();
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {

            c = *iter++;
            orbs[i][j].color = EMPTY;

            if(c == 'R')      orbs[i][j].color = R;
            else if(c == 'G') orbs[i][j].color = G;
            else if(c == 'B') orbs[i][j].color = B;
            else if(c == 'L') orbs[i][j].color = L;
            else if(c == 'D') orbs[i][j].color = D;
            else if(c == 'H') orbs[i][j].color = H;
            else if(c == 'J') orbs[i][j].color = J;
            else if(c == 'P') orbs[i][j].color = P;
            else {
                std::cerr << "Error loading orb code: invalid character " << c << std::endl;
            }
        }
    }

    setStartLocations();
}

void Board::swapOrbs(int x1, int y1, int x2, int y2)
{
    orb tmp;

    if(x1 == x2 && y1 == y2)
        return;

    tmp = orbs[x1][y1];
    orbs[x1][y1] = orbs[x2][y2];
    orbs[x2][y2] = tmp;

    evaluated = false;
}

bool Board::removeComboOne()
{
    int i, j;

    // no combos
    if(findCombos() == 0)
        return false;

    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            // only remove 1st combo
            if(comboBoard[i][j] == 1)
                orbs[i][j].color = EMPTY;

    return true;
}

void Board::removeComboAll()
{
    int i, j;

    // no combos
    if(findCombos() == 0)
        return;

    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            if(comboBoard[i][j] != 0)
                orbs[i][j].color = EMPTY;
}

bool Board::dropDownOne()
{
    int i, j;
    bool dropped = false;

    for(i=0; i<BOARD_X; i++) {
        for(j=BOARD_Y-1; j>0; j--) {
            if(orbs[i][j] == EMPTY && orbs[i][j-1] != EMPTY) {
                swapOrbs(i, j, i, j-1);
                dropped = true;
            }
        }
    }

    return dropped;
}

void Board::dropDownAll()
{
    int i;
    int in_y, out_y;

    // loop over columns
    for(i=0; i<BOARD_X; i++) {
        // loop bottom to top
        out_y = BOARD_Y-1;
        for(in_y=BOARD_Y-1; in_y>=0; in_y--) {
            // skip over empty spots
            if(orbs[i][in_y] == EMPTY) continue;
            // swap the orbs
            swapOrbs(i, out_y, i, in_y);
            // move output location up
            out_y--;
        }
    }
}

void Board::fillEmpty()
{
    int i, j;

    for(i=0; i<BOARD_X; i++) {
        for(j=0; j<BOARD_Y; j++) {
            if(orbs[i][j] == EMPTY)
                orbs[i][j].color = makeRandomOrb();
        }
    }
}

void Board::evaluateBoard()
{
    int i, j;

    float total_tightness = 0;
    float total_closeness = 0;
    float onetime_closeness;
    float onetime_tightness;
    int orbcount;

    int n_combos;
    std::vector<Combo> thisLevelCombos;

    orb original_board[BOARD_X][BOARD_Y];

    if(evaluated)
        return;

    // initialize
    combos   = 0;
    cascades = 0;
    score    = 0;
    comboList.clear();

    // save board
    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            original_board[i][j] = orbs[i][j];

    // loop through all cascade levels
    while(hasMatches()) {
        n_combos = findCombos();
        combos += n_combos;
        cascades++;

        // add combos to combo list
        thisLevelCombos.clear();
        for(i=1; i<=n_combos; i++)
            thisLevelCombos.push_back(Combo(comboBoard, orbs, i));
        comboList.push_back(thisLevelCombos);

        // find tightness of combo packing
        // counts orbs either part of a combo or not next to any combo
        onetime_tightness = 0;
        for(i=0; i<BOARD_X; i++) {
            for(j=0; j<BOARD_Y; j++) {
                if(orbs[i][j] != EMPTY) {
                    if(comboBoard[i][j] != 0) onetime_tightness += 1;
                    else {
                        if((i-1 < 0        || comboBoard[i-1][j] == 0)
                        && (i+1 >= BOARD_X || comboBoard[i+1][j] == 0)
                        && (j-1 < 0        || comboBoard[i][j-1] == 0)
                        && (j+1 >= BOARD_Y || comboBoard[i][j+1] == 0))
                            onetime_tightness += 1;
                    }
                }
            }
        }
        total_tightness += onetime_tightness / 30;

        // remove combos of this cascade level
        removeComboAll();
        dropDownAll();
    }

     // find closeness of unmatched orbs
    // loop through all colors
    for(int color=1; color<=NUM_ORBTYPES; color++) {
        onetime_closeness = 0;
        orbcount = 0;

        for(i=0; i<BOARD_X; i++) {
            for(j=0; j<BOARD_Y; j++) {
                if(orbs[i][j] == color) {
                    orbcount += 1;
                    // check 4 directions
                    if(i-1 >= 0      && orbs[i-1][j] == color)
                        onetime_closeness += 1;
                    if(i+1 < BOARD_X && orbs[i+1][j] == color)
                        onetime_closeness += 1;
                    if(j-1 >= 0      && orbs[i][j-1] == color)
                        onetime_closeness += 1;
                    if(j+1 < BOARD_Y && orbs[i][j+1] == color)
                        onetime_closeness += 1;
                    // check diagonals
                    if(i-1 >= 0      && j-1 >= 0      && orbs[i-1][j-1] == color)
                        onetime_closeness += 0.5;
                    if(i-1 >= 0      && j+1 < BOARD_Y && orbs[i-1][j+1] == color)
                        onetime_closeness += 0.5;
                    if(i+1 < BOARD_X && j-1 >= 0      && orbs[i+1][j-1] == color)
                        onetime_closeness += 0.5;
                    if(i+1 < BOARD_X && j+1 < BOARD_Y && orbs[i+1][j+1] == color)
                        onetime_closeness += 0.5;
                }
            }
        }

        if(orbcount > 0)
            total_closeness += onetime_closeness / (6*orbcount);
    }


    // restore board
    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            orbs[i][j] = original_board[i][j];

    // correct the cascade count
    cascades = cascades-1 < 0? 0 : cascades - 1;

    // start calculating score
    score = combos;
    score += 0.25 * cascades;
    score += 0.2 * total_tightness;
    score += 0.1 * total_closeness;
    // random for tie breaker
    score += 0.0001 * ((double)std::rand() / (double)RAND_MAX);

    evaluated = true;
}

std::vector<std::vector<Combo>> Board::getCombos()
{
    if(!evaluated)
        evaluateBoard();

    return comboList;
}

bool Board::hasMatches()
{
    int i, j;

    // check matches in the x direction
    for(j=0; j<BOARD_Y; j++)
        for(i=0; i<BOARD_X-2; i++)
            if(orbs[i][j] != EMPTY && orbs[i][j] == orbs[i+1][j] && orbs[i+1][j] == orbs[i+2][j])
                return true;

    // check matches in the y direction
    for(j=0; j<BOARD_Y-2; j++)
        for(i=0; i<BOARD_X; i++)
            if(orbs[i][j] != EMPTY && orbs[i][j] == orbs[i][j+1] && orbs[i][j+1] == orbs[i][j+2])
                return true;

    return false;
}

bool Board::filterMatches()
{
    int i, j;
    bool has_matches = false;
    bool mark[BOARD_X][BOARD_Y];

    // initialize
    for(i=0; i<BOARD_X; i++) {
        for(j=0; j<BOARD_Y; j++) {
            matchBoard[i][j].color = EMPTY;
            mark[i][j] = false;
        }
    }

    // copy the orbs over
    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            matchBoard[i][j] = orbs[i][j];

    // mark matches in the x direction
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X-2; i++) {
            if(orbs[i][j] != EMPTY && orbs[i][j] == orbs[i+1][j] && orbs[i+1][j] == orbs[i+2][j]) {
                has_matches  = true;
                mark[i][j]   = true;
                mark[i+1][j] = true;
                mark[i+2][j] = true;
            }
        }
    }

    // mark matches in the y direction
    for(j=0; j<BOARD_Y-2; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(orbs[i][j] != EMPTY && orbs[i][j] == orbs[i][j+1] && orbs[i][j+1] == orbs[i][j+2]) {
                has_matches  = true;
                mark[i][j]   = true;
                mark[i][j+1] = true;
                mark[i][j+2] = true;
            }
        }
    }

    // remove non marked orbs
    for(j=0; j<BOARD_Y; j++)
        for(i=0; i<BOARD_X; i++)
            if(mark[i][j] == false)
                matchBoard[i][j].color = EMPTY;

    return has_matches;
}

bool Board::findFirstNonempty(int *x, int *y)
{
    int i, j;

    *x = -1;
    *y = -1;

    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(matchBoard[i][j] != EMPTY) {
                *x = i;
                *y = j;
                return true;
            }
        }
    }

    return false;
}

void Board::floodFill(int x, int y, int fillNumber)
{
    int i,j;
    int color;
    bool visited[BOARD_X][BOARD_Y];

    // initialize array tracking visited spaces
    for(j=0; j<BOARD_Y; j++)
        for(i=0; i<BOARD_X; i++)
            visited[i][j] = 0;

    color = matchBoard[x][y].color;
    comboBoard[x][y] = fillNumber;
    visited[x][y] = 1;

    floodFillRecursive(x+1, y, fillNumber, color, visited);
    floodFillRecursive(x-1, y, fillNumber, color, visited);
    floodFillRecursive(x, y+1, fillNumber, color, visited);
    floodFillRecursive(x, y-1, fillNumber, color, visited);
}

void Board::floodFillRecursive(int x, int y, int fillNumber, int orbColor, bool visited[BOARD_X][BOARD_Y])
{
    // base cases
    if(x < 0 || x >= BOARD_X) return;
    if(y < 0 || y >= BOARD_Y) return;
    if(visited[x][y]) return;
    visited[x][y] = true;

    // wrong color
    if(matchBoard[x][y] != orbColor) return;

    // fill this space and adjacent spaces
    comboBoard[x][y] = fillNumber;
    floodFillRecursive(x+1, y, fillNumber, orbColor, visited);
    floodFillRecursive(x-1, y, fillNumber, orbColor, visited);
    floodFillRecursive(x, y+1, fillNumber, orbColor, visited);
    floodFillRecursive(x, y-1, fillNumber, orbColor, visited);

}

int Board::findCombos()
{
    int i, j;
    int x, y;
    int n_combos = 0;

    // filter matches
    // if no combos, quit
    if(filterMatches() == false)
        return 0;

    // initialize
    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            comboBoard[i][j] = 0;

    // find nonempty space
    while(findFirstNonempty(&x, &y)) {
        n_combos++;

        // fill the combo
        floodFill(x, y, n_combos);

        // remove combo from matchBoard;
        for(i=0; i<BOARD_X; i++)
            for(j=0; j<BOARD_Y; j++)
                if(comboBoard[i][j] == n_combos)
                    matchBoard[i][j].color = EMPTY;
    }

    return n_combos;
}

void Board::setStartLocations()
{
    int i, j;

    for(i=0; i<BOARD_X; i++) {
        for(j=0; j<BOARD_Y; j++) {
            orbs[i][j].startX = i;
            orbs[i][j].startY = j;
        }
    }
}

void Board::setFinishLocations()
{
    int i, j;

    for(i=0; i<BOARD_X; i++) {
        for(j=0; j<BOARD_Y; j++) {
            orbs[i][j].finishX = i;
            orbs[i][j].finishY = j;
        }
    }
}

std::ostream &operator<<(std::ostream &os, Board const &b)
{
    int i,j;

    // print top border
    for(i=0; i<BOARD_X; i++)
        os << "-";
    os << "\n";

    // print rows
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(b.orbs[i][j] == EMPTY) os << " ";
            if(b.orbs[i][j] == R) os << "R";
            if(b.orbs[i][j] == G) os << "G";
            if(b.orbs[i][j] == B) os << "B";
            if(b.orbs[i][j] == L) os << "L";
            if(b.orbs[i][j] == D) os << "D";
            if(b.orbs[i][j] == H) os << "H";
            if(b.orbs[i][j] == J) os << "J";
            if(b.orbs[i][j] == P) os << "P";
        }
        os << "\n";
    }

    // print bottom border
    for(i=0; i<BOARD_X; i++)
        os << "-";
    os << "\n";

    // print orb code
    os << "Dawnglare code: ";
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(b.orbs[i][j] == EMPTY) os << " ";
            if(b.orbs[i][j] == R) os << "R";
            if(b.orbs[i][j] == G) os << "G";
            if(b.orbs[i][j] == B) os << "B";
            if(b.orbs[i][j] == L) os << "L";
            if(b.orbs[i][j] == D) os << "D";
            if(b.orbs[i][j] == H) os << "H";
            if(b.orbs[i][j] == J) os << "J";
            if(b.orbs[i][j] == P) os << "P";
        }
    }
    return os << "\n";
}
