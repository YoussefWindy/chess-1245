// src/pieces/rook.cc

#include "../../include/board.h"
#include "../../include/pieces/rook.h"

Rook::Rook(bool colour, const Posn &posn):
  Piece{colour ? 'R' : 'r', colour, posn, true, true, false, false} {}

void Rook::calculateLegalMoves(const Board &board) {
    // Fill in
}
