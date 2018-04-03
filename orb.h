#ifndef ORB_H
#define ORB_H


#define NUM_ORBTYPES 8
enum orbcolor {
    EMPTY = 0,
    R,
    G,
    B,
    L,
    D,
    H,
    J,
    P
};

struct orb {
    // color or empty
    int color;

    // location before movement
    int startX;
    int startY;

    // location immediately after all movement finishes
    int finishX;
    int finishY;
};

int makeRandomOrb();

bool operator==(const orb &a, const orb &b);
bool operator==(const orb &a, const int &b);
bool operator==(const int &a, const orb &b);
bool operator!=(const orb &a, const orb &b);
bool operator!=(const orb &a, const int &b);
bool operator!=(const int &a, const orb &b);


#endif // ORB_H
