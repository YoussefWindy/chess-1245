// src/pieces/rook.cc

#include "../../include/board.h"
#include "../../include/pieces/rook.h"

Rook::Rook(bool colour, const Posn &posn):
  Piece{colour ? 'R' : 'r', colour, 5, posn, true, true, false, false} {}

void Rook::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((!i && !j) || (i && j) || (!vertical && j) || (!horizontal && i)) continue;
            for (int k = 1;; k++) {
                try {
                    Posn p{posn.x + i * k, posn.y + j * k};
                    if (!board[p]) {
                        legalMoves.emplace_back(p);
                    } else if (board[p]->getColour() != colour) {
                        legalMoves.emplace_back(p);
                        if (board[p]->getName() != (!colour ? 'K' : 'k')) break;
                    } else {
                        board[p]->protect();
                        break;
                    }
                } catch (BadPosn &e) {
                    break;
                }
            }
        }
    }
    vertical = horizontal = true;
}
