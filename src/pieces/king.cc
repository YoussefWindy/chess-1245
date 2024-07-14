// src/pieces/king.cc

#include "../../include/pieces/king.h"

King::King(bool colour, const Posn &posn): Piece{colour, colour ? "K" : "k", posn} {}

void King::calculateLegalMoves(const Board &board) {
    // Fill in
}
