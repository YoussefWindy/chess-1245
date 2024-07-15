// src/pieces/knight.cc

#include "../../include/board.h"
#include "../../include/pieces/knight.h"

Knight::Knight(bool colour, const Posn &posn):
  Piece{colour ? 'N' : 'n', colour, posn, false, false, false, false} {}

void Knight::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    if (board.isPinned(posn)) return;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            unsigned int x = posn.x + !(i % 2) ? i ? -2 : 2 : j ? -1 : 1;
            unsigned int y = posn.y + i % 2 ? i > 1 ? -2 : 2 : j ? -1 : 1;
            try {
                Posn p{x, y};
                if (board[p]) {
                    if (board[p]->getColour() != colour) {
                        legalMoves.emplace_back(p);
                    } else {
                        board[p]->protect();
                    }
                }
            } catch (BadPosn &e) {}
        }
    }
}
