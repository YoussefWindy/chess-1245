// src/pieces/rook.cc

#include "../../include/board.h"
#include "../../include/pieces/rook.h"

Rook::Rook(bool colour, const Posn &posn):
  Piece{colour ? 'R' : 'r', colour, posn, true, true, false, false} {}

void Rook::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (board.isPinned(posn)) return;
    if (vertical) {
        for (int i = 1; posn.y + i < 8; i++) {
            if (board[Posn{posn.x, posn.y + i}]) {
                legalMoves.emplace_back(Posn{posn.x, posn.y + i});
            } else if (board[Posn{posn.x, posn.y + i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x, posn.y + i});
                break;
            } else {
                break;
            }
        }
        for (int i = 1; posn.y - i >= 0; i++) {
            if (board[Posn{posn.x, posn.y - i}]) {
                legalMoves.emplace_back(Posn{posn.x, posn.y - i});
            } else if (board[Posn{posn.x, posn.y - i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x, posn.y - i});
                break;
            } else {
                break;
            }
        }
    }
    if (horizontal) {
        for (int i = 1; posn.x + i < 8; i++) {
            if (board[Posn{posn.x + i, posn.y}]) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y});
            } else if (board[Posn{posn.x + i, posn.y}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y});
                break;
            } else {
                break;
            }
        }
        for (int i = 1; posn.x - i >= 0; i++) {
            if (board[Posn{posn.x - i, posn.y}]) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y});
            } else if (board[Posn{posn.x - i, posn.y}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y});
                break;
            } else {
                break;
            }
        }
    }
    vertical = true;
    horizontal = true;
}
