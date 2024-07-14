// src/pieces/queen.cc

#include "../../include/pieces/queen.h"

Queen::Queen(bool colour, const Posn &posn): Piece{colour, colour ? "Q" : "q", posn} {}

void Queen::calculateLegalMoves(const Board &board) {
    // Fill in
}
