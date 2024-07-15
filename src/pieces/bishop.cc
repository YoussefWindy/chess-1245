// src/pieces/bishop.cc

#include "../../include/board.h"
#include "../../include/pieces/bishop.h"

Bishop::Bishop(bool colour, const Posn &posn): Piece{colour ? 'B' : 'b', colour, posn} {}

void Bishop::calculateLegalMoves(const Board &board) {
    // Fill in
}

int Bishop::calculateNumPinned(const Board &board) const {
    // Fill in
}
