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
    int calculateNumPins(const Board  &board) const;
};

class Rook final: public Piece {
    bool hasMoved = false;
  public:
    Rook(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

class Queen final: public Piece {
  public:
    Queen(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

class King final: public Piece {
    bool hasMoved = false;
  public:
    King(bool colour, const Posn &posn);
    void calculateLegalMoves(const Board &board) override;
    int calculateNumPins(const Board  &board) const;
};

#endif // PIECES_H
