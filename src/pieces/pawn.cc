// src/pieces/pawn.cc

#include "../../include/board.h"
#include "../../include/pieces/pawn.h"

Pawn::Pawn(bool colour, const Posn &posn): Piece{colour ? 'P' : 'p', colour, posn}, promotable{false} {}

void Pawn::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (board.isPinned(posn)) return;
    if (colour) { // if white
        if (!board[{posn.x, posn.y + 1}]) { // if space in front is free
            legalMoves.emplace_back(posn.x, posn.y + 1); // can move forward one space
            if (posn.y < HEIGHT - 2 && !hasMoved && !board[{posn.x, posn.y + 2}]) { // if two spaces ahead is free
                legalMoves.emplace_back(posn.x, 3); // double movement if on starting space
            }
        }
        if (posn.x && board[{posn.x - 1, posn.y + 1}]) {
            if (!board[{posn.x - 1, posn.y + 1}]->getColour()) { // normal capture left
                legalMoves.emplace_back(posn.x - 1, posn.y + 1); // can capture left
            } else {
                board[{posn.x - 1, posn.y + 1}]->protect(); // is protecting left
            }
        } else if (posn.x && posn.y == HEIGHT - 4 && board[{posn.x - 1, HEIGHT - 4}] && board[{posn.x - 1, HEIGHT - 4}]->getName() == 'p' // en passant left
          && board.getLastMove() == Move{{posn.x - 1, HEIGHT - 2}, {posn.x - 1, HEIGHT - 4}}) {

        }
        if (posn.x < WIDTH - 1 && ((!board[{posn.x + 1, posn.y + 1}]->getColour()) // normal capture right
          || (posn.y == HEIGHT - 4 && board[{posn.x + 1, HEIGHT - 4}]->getName() == 'p' // en passant right
          && board.getLastMove() == Move{{posn.x + 1, HEIGHT - 2}, {posn.x + 1, HEIGHT - 4}}))) {
            legalMoves.emplace_back(posn.x + 1, posn.y + 1); // can capture right
        }
    } else {
        if (!board[{posn.x, posn.y - 1}]) { // if black and space in front is free
            legalMoves.emplace_back(posn.x, posn.y - 1); // can move forward one space
            if (posn.y > 1 && !hasMoved && !board[{posn.x, posn.y - 2}]) { // if two spaces ahead is free
                legalMoves.emplace_back(posn.x, HEIGHT - 4); // double movement if on starting space
            }
        }
        if (posn.x && ((!board[{posn.x - 1, posn.y - 1}]->getColour()) // normal capture left
          || (posn.y == 3 && board[{posn.x - 1, 3}]->getName() == 'P' // en passant left
          && board.getLastMove() == Move{{posn.x - 1, 1}, {posn.x - 1, 3}}))) {
            legalMoves.emplace_back(posn.x - 1, posn.y - 1); // can capture left
        }
        if (posn.x < WIDTH - 1 && ((!board[{posn.x + 1, posn.y - 1}]->getColour()) // normal capture right
          || (posn.y == 3 && board[{posn.x + 1, 3}]->getName() == 'P' // en passant right
          && board.getLastMove() == Move{{posn.x + 1, 1}, {posn.x + 1, 3}}))) {
            legalMoves.emplace_back(posn.x + 1, posn.y - 1); // can capture right
        }
    }
    if (!legalMoves.empty()) promotable = posn.y == HEIGHT - 2;
}

int Pawn::calculateNumPinned(const Board &board) const {
    return 0; // pawns can't pin lol
}

bool Pawn::canPromote() const {
    return promotable;
}
