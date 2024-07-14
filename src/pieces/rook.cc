// src/pieces/rook.cc

#include "../../include/pieces/rook.h"

Rook::Rook(bool colour, const Posn &posn): Piece{colour, colour ? "R" : "r", posn} {}

void Rook::calculateLegalMoves(const Board &board) {
    // Fill in
}
