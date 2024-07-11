// src/pieces/knight.cc

#include "../../include/pieces.h"

Knight::Knight(bool colour, const Posn &posn): Piece{colour, colour ? "N" : "n", posn} {}

void Knight::calculateLegalMoves(const Board &board) {
    // Fill in
}
