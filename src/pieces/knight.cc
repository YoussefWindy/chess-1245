// src/pieces/knight.cc

#include "../../include/pieces.h"

Knight::Knight(bool colour, const Posn &posn): Piece{colour, colour ? "N" : "n", posn} {}

void Knight::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (board.isPinned(posn)) return;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            int x = posn.x + !(i % 2) ? i ? -2 : 2 : j ? -1 : 1;
            int y = posn.y + i % 2 ? i > 1 ? -2 : 2 : j ? -1 : 1;
            try {
                Posn p{x, y};
                if (board[p] && board[p]->getColour() != colour) {
                    legalMoves.emplace_back(p);
                }
            } catch (BadPosn &e) {}
        }
    }
}
