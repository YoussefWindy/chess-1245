// src/pieces/bishop.cc

#include "../../include/board.h"
#include "../../include/pieces/bishop.h"

Bishop::Bishop(bool colour, const Posn &posn):
  Piece{colour ? 'B' : 'b', colour, posn, false, false, true, true} {}

void Bishop::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i == 0 || j == 0) continue;
            if (!positive && ((i == 1 && j == 1) || (i == -1 && j == -1))) continue;
            if (!negative && ((i == 1 && j == -1) || (i == -1 && j == 1))) continue;
            for (int k = 1; k < 8; ++k) {
                try {
                    Posn p{posn.x + i * k, posn.y + j * k};
                    if (!board[p]) {
                        legalMoves.emplace_back(p);
                    } else if (board[p]->getColour() != colour) {
                        legalMoves.emplace_back(p);
                        break;
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
    positive = negative = true;
}
