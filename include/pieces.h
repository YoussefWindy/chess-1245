// include/pieces.h
#ifndef PIECES_H
#define PIECES_H

#include "piece.h"

class Pawn final: public Piece {
  public:
    Pawn(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

class Knight final: public Piece {
  public:
    Knight(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

class Bishop final: public Piece {
  public:
    Bishop(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

class Rook final: public Piece {
  public:
    Rook(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

class Queen final: public Piece {
  public:
    Queen(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

class King final: public Piece {
  public:
    King(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
};

#endif // PIECES_H
