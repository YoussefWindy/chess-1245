// src/pieces/rook.cc

#include "../../include/pieces/rook.h"

Rook::Rook(bool colour, const Posn &posn): Piece{colour ? 'R' : 'r', colour, posn} {}

void Rook::calculateLegalMoves(const Board &board) {
    // Fill in
}
