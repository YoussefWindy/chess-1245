// include/pieces/bishop.h
#ifndef BISHOP_H
#define BISHOP_H

#include "../piece.h"

class Bishop final: public Piece {
  public:
    Bishop(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

#endif // BISHOP_H
