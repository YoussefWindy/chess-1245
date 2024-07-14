// include/utilities/badmove.h
#ifndef BADMOVE_H
#define BADMOVE_H

#include "move.h"

struct BadMove: public std::exception {
    Move move;
    BadMove(const Move &move);
};

#endif // BADMOVE_H
