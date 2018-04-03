#ifndef COMBO_H
#define COMBO_H

#include "settings.h"
#include "orb.h"

class Combo
{
public:
    int n_orbs;
    orb orbs[BOARD_X][BOARD_Y];

    Combo();
    Combo(int comboBoard[BOARD_X][BOARD_Y], orb boardOrbs[BOARD_X][BOARD_Y], int comboNumber);

};

#endif // COMBO_H
