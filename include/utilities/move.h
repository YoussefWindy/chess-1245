// include/utilities/move.h
#ifndef MOVE_H
#define MOVE_H

#include "posn.h"

struct Move {
    Posn oldPos, newPos;
    Move(Posn o = {0, 0}, Posn n = {0, 0});
    bool operator==(const Move &other) const; // This one might be unnecessary, can remove if we don't use it
};

#endif // MOVE_H
