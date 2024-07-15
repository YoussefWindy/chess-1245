// include/pieces/rook.h
#ifndef ROOK_H
#define ROOK_H

#include "../piece.h"

class Rook final: public Piece {
  public:
    Rook(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

#endif // ROOK_H
