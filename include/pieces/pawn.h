// include/pieces/pawn.h
#ifndef PAWN_H
#define PAWN_H

#include "../piece.h"

class Pawn final: public Piece {
  public:
    Pawn(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

#endif // PAWN_H
