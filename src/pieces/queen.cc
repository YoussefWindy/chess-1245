// src/pieces/queen.cc

#include "../../include/board.h"
#include "../../include/pieces/queen.h"

Queen::Queen(bool colour, const Posn &posn):
  Piece{colour ? 'Q' : 'q', colour, posn, true, true, true, true} {}

void Queen::calculateLegalMoves(const Board &board) {
    // Fill in
}
