// include/utilities/badposn.h
#ifndef BADPOSN_H
#define BADPOSN_H

#include "posn.h"

struct BadPosn: public std::exception {
    Posn posn;
    BadPosn(const Posn &posn);
};

#endif // BADPOSN_H
