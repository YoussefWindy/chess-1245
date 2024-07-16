// src/pieces/king.cc

#include "../../include/board.h"
#include "../../include/pieces/king.h"

King::King(bool colour, const Posn &posn):
  Piece{colour ? 'K' : 'k', colour, posn, true, true, true, true} {}

void King::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!i && !j) continue; // if i = j = 0, this means we are checking the square the king is currently on
            try {
                Posn p{posn.x + i, posn.y + j};
                if ((!board[p] || (board[p]->getColour() != colour // if square is empty, or square is not belonging to us already
                  && board[p]->getIsProtected())) && !board.inCheck(p, colour)) { // AND not protected, and square is not in check
                    legalMoves.emplace_back(p);
                }
            } catch (BadPosn &e) {}
        }
    }
    if (!hasMoved && !board.inCheck(posn, colour)) { // if king hasn't moved yet and is not in check
        for (unsigned int i = 0; i < WIDTH; i++) { // look at all pieces of this's colour on this's row
            std::shared_ptr<Piece> p = board[{i, posn.y}];
            if (p->getName() == (colour ? 'R' : 'r') && !p->getHasMoved() && // if p is a rook that hasn't moved yet and is at the same Y level as this
              posn.y == p->getY() && (posn.x - p->getX() == 4 || p->getX() - posn.x == 3)) { // and is either 4 spaces to the left or 3 to the right of this
                bool pathInCheck = false;
                for (int j = 1; j < (posn.x < p->getX() ? 3 : 4); j++) { // making sure there are no pieces in between and that
                    Posn pos{posn.x + (posn.x < p->getX() ? j : -j), posn.y}; // none of the positions in between are in check
                    if (board[pos] || board.inCheck(pos, colour)) {
                        pathInCheck = true;
                        break;
                    }
                }
                if (!pathInCheck && !board.inCheck(p->getPosn(), colour)) { // if none of the spaces in between are in check and the rook isn't in check either
                    legalMoves.emplace_back(Posn{posn.x + (posn.x < p->getX() ? 2 : -2), posn.y});
                }
            }
        }
    }
}

void King::calculatePins(const Board &board) {
    
}
