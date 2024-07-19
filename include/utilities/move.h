// include/utilities/move.h
#ifndef MOVE_H
#define MOVE_H

#include "posn.h"

struct Move {
    Posn oldPos, newPos;
    bool capture;
    unsigned int promotion;
    Move(Posn o, Posn n, bool c = false, unsigned int p = 0);
    bool operator==(const Move &other) const;
};

#endif // MOVE_H
