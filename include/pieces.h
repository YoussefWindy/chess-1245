// include/pieces.h
#ifndef PIECES_H
#define PIECES_H

#include "piece.h"

class Pawn: public Piece {
  public:
    void calculateLegalMoves() override;
};

class Knight: public Piece {
  public:
    void calculateLegalMoves() override;
};

class Bishop: public Piece {
  public:
    void calculateLegalMoves() override;
};

class Rook: public Piece {
  public:
    void calculateLegalMoves() override;
};

class Queen: public Piece {
  public:
    void calculateLegalMoves() override;
};

class King: public Piece {
  public:
    void calculateLegalMoves() override;
};

#endif // PIECES_H
