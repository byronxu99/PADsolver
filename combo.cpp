#include "combo.h"

Combo::Combo()
{
    int i, j;

    n_orbs = 0;

    for(i=0; i<BOARD_X; i++)
        for(j=0; j<BOARD_Y; j++)
            orbs[i][j].color = EMPTY;
}

Combo::Combo(int comboBoard[BOARD_X][BOARD_Y], orb boardOrbs[BOARD_X][BOARD_Y], int comboNumber)
{
    int i, j;

    n_orbs = 0;

    for(i=0; i<BOARD_X; i++) {
        for(j=0; j<BOARD_Y; j++) {
            if(comboBoard[i][j] == comboNumber) {
                orbs[i][j] = boardOrbs[i][j];
                n_orbs++;
            } else {
                orbs[i][j].color = EMPTY;
            }
        }
    }
}
