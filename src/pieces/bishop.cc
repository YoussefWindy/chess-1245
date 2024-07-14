// src/pieces/bishop.cc

#include "../../include/pieces/bishop.h"

Bishop::Bishop(bool colour, const Posn &posn): Piece{colour, colour ? "B" : "b", posn} {}

void Bishop::calculateLegalMoves(const Board &board) {
    // Fill in
}