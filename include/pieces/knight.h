// include/pieces/knight.h
#ifndef KNIGHT_H
#define KNIGHT_H

#include "../piece.h"

class Knight final: public Piece {
  public:
    Knight(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPinned(const Board &board) const override;
};

#endif // KNIGHT_H
