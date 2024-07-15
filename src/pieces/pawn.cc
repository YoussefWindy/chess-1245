// src/pieces/pawn.cc

#include "../../include/board.h"
#include "../../include/pieces/pawn.h"

Pawn::Pawn(bool colour, const Posn &posn): Piece{colour ? 'P' : 'p', colour, posn} {}

void Pawn::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (board.isPinned(posn)) return;
    if (colour) { // if white
        if (!board[{posn.x, posn.y + 1}]) { // if space in front is free
            legalMoves.emplace_back(posn.x, posn.y + 1); // can move forward one space
            if (posn.y == 1 && !legalMoves.empty() && !board[{posn.x, posn.y + 2}]) { // if two spaces ahead is free
                legalMoves.emplace_back(posn.x, 3); // double movement if on starting space
            }
        }
        if ((posn.x && (!board[{posn.x - 1, posn.y + 1}]->getColour())) // normal capture left
          || (posn.y == 4 && board[{posn.x - 1, 4}]->getName() == 'p' // en passant left
          && board.getLastMove() == Move{{posn.x - 1, 6}, {posn.x - 1, 4}})) {
            legalMoves.emplace_back(posn.x - 1, posn.y + 1); // can capture left
        }
        if ((posn.x < WIDTH - 1 && (!board[{posn.x + 1, posn.y + 1}]->getColour())) // normal capture right
          || (posn.y == 4 && board[{posn.x + 1, 4}]->getName() == 'p' // en passant right
          && board.getLastMove() == Move{{posn.x + 1, 6}, {posn.x + 1, 4}})) {
            legalMoves.emplace_back(posn.x + 1, posn.y + 1); // can capture right
        }
    } else {
        if (!board[{posn.x, posn.y - 1}]) { // if black and space in front is free
            legalMoves.emplace_back(posn.x, posn.y - 1); // can move forward one space
            if (posn.y == 6 && !legalMoves.empty() && !board[{posn.x, posn.y - 2}]) { // if two spaces ahead is free
                legalMoves.emplace_back(posn.x, 4); // double movement if on starting space
            }
        }
        if ((posn.x && (!board[{posn.x - 1, posn.y - 1}]->getColour())) // normal capture left
          || (posn.y == 3 && board[{posn.x - 1, 3}]->getName() == 'P' // en passant left
          && board.getLastMove() == Move{{posn.x - 1, 1}, {posn.x - 1, 3}})) {
            legalMoves.emplace_back(posn.x - 1, posn.y - 1); // can capture left
        }
        if ((posn.x < WIDTH - 1 && (!board[{posn.x + 1, posn.y - 1}]->getColour())) // normal capture right
          || (posn.y == 3 && board[{posn.x + 1, 3}]->getName() == 'P' // en passant right
          && board.getLastMove() == Move{{posn.x + 1, 1}, {posn.x + 1, 3}})) {
            legalMoves.emplace_back(posn.x + 1, posn.y - 1); // can capture right
        }
    }
}

int Pawn::calculateNumPinned(const Board &board) const {
    return 0;
}
