// include/pieces/rook.h
#ifndef ROOK_H
#define ROOK_H

#include "../piece.h"

class Rook final: public Piece {
    bool hasMoved = false;
  public:
    Rook(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

#endif // ROOK_H
