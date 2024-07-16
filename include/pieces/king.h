// include/pieces/king.h
#ifndef KING_H
#define KING_H

#include "../piece.h"

class King final: public Piece {
  public:
    King(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    void calculatePins(const Board &board);
};

#endif // KING_H
