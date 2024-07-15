// src/pieces/king.cc

#include "../../include/board.h"
#include "../../include/pieces/king.h"

King::King(bool colour, const Posn &posn): Piece{colour ? 'K' : 'k', colour, posn} {}

void King::calculateLegalMoves(const Board &board) {
    
}

int King::calculateNumPinned(const Board &board) const {
    return 0;
}
