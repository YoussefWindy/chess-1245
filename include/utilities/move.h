// include/utilities/move.h
#ifndef MOVE_H
#define MOVE_H

#include "posn.h"

struct Move {
    Posn oldPos, newPos;
    unsigned int promotion;
    bool capture;
    Move(Posn o, Posn n, unsigned int p = 0, bool c = false);
    bool operator==(const Move &other) const;
};

#endif // MOVE_H
