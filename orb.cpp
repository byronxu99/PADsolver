#include <cstdlib>

#include "orb.h"


int makeRandomOrb()
{
    return (rand() % 6) + 1;
}


bool operator==(const orb &a, const orb &b)
{
    return a.color == b.color;
}

bool operator==(const orb &a, const int &b)
{
    return a.color == b;
}

bool operator==(const int &a, const orb &b)
{
    return a == b.color;
}

bool operator!=(const orb &a, const orb &b)
{
    return a.color != b.color;
}

bool operator!=(const orb &a, const int &b)
{
    return a.color != b;
}

bool operator!=(const int &a, const orb &b)
{
    return a != b.color;
}
