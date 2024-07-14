// src/pieces/queen.cc

#include "../../include/board.h"
#include "../../include/pieces/queen.h"

Queen::Queen(bool colour, const Posn &posn): Piece{colour ? 'Q' : 'q', colour, posn} {}

void Queen::calculateLegalMoves(const Board &board) {
    // Fill in
}

int Queen::calculateNumPinned(const Board &board) const {
    // Fill in
}
