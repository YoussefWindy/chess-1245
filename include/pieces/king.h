// include/pieces/king.h
#ifndef KING_H
#define KING_H

#include "../piece.h"

class King final: public Piece {
    bool hasMoved = false;
  public:
    King(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPinned(const Board &board) const override;
};

#endif // KING_H
