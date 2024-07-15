// include/pieces/pawn.h
#ifndef PAWN_H
#define PAWN_H

#include "../piece.h"

class Pawn final: public Piece {
    bool promotable;
  public:
    Pawn(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    bool canPromote() const;
};

#endif // PAWN_H
