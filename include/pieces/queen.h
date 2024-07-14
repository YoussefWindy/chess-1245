// include/pieces/queen.h
#ifndef QUEEN_H
#define QUEEN_H

#include "../piece.h"

class Queen final: public Piece {
  public:
    Queen(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

#endif // QUEEN_H
