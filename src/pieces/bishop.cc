// src/pieces/bishop.cc

#include "../../include/board.h"
#include "../../include/pieces/bishop.h"

Bishop::Bishop(bool colour, const Posn &posn):
  Piece{colour ? 'B' : 'b', colour, 3, posn, false, false, true, true} {}

void Bishop::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (positive) {
        for (int i = 1; posn.x + i < 8 && posn.y + i < 8; i++) {
            if (board[Posn{posn.x + i, posn.y + i}]) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y + i});
            } else if (board[Posn{posn.x + i, posn.y + i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y + i});
                break;
            } else {
                break;
            }
        }
        for (int i = 1; posn.x - i >= 0 && posn.y - i >= 0; i++) {
            if (board[Posn{posn.x - i, posn.y - i}]) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y - i});
            } else if (board[Posn{posn.x - i, posn.y - i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y - i});
                break;
            } else {
                break;
            }
        }
    }
    if (negative) {
        for (int i = 1; posn.x + i < 8 && posn.y - i >= 0; i++) {
            if (board[Posn{posn.x + i, posn.y - i}]) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y - i});
            } else if (board[Posn{posn.x + i, posn.y - i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x + i, posn.y - i});
                break;
            } else {
                break;
            }
        }
        for (int i = 1; posn.x - i >= 0 && posn.y + i < 8; i++) {
            if (board[Posn{posn.x - i, posn.y + i}]) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y + i});
            } else if (board[Posn{posn.x - i, posn.y + i}]->getColour() != colour) {
                legalMoves.emplace_back(Posn{posn.x - i, posn.y + i});
                break;
            } else {
                break;
            }
        }
    }
    positive = negative = true;
}
