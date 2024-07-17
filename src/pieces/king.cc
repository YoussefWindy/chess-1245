// src/pieces/king.cc

#include "../../include/board.h"
#include "../../include/pieces/king.h"

King::King(bool colour, const Posn &posn):
  Piece{colour ? 'K' : 'k', colour, 10, posn, true, true, true, true} {}

void King::calculateLegalMoves(const Board &board) {
    legalMoves.clear();
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!i && !j) continue; // if i = j = 0, this means we are checking the square the king is currently on
            try {
                Posn p{posn.x + i, posn.y + j};
                if ((!board[p] || (board[p]->getColour() != colour // if square is empty, or square is not belonging to us already
                  && board[p]->getIsProtected())) && !board.check(p, colour)) { // AND not protected, and square is not in check
                    legalMoves.emplace_back(p);
                }
            } catch (BadPosn &e) {}
        }
    }
    if (!hasMoved && !board.check(posn, colour)) { // if king hasn't moved yet and is not in check
        for (unsigned int i = 0; i < WIDTH; i++) { // look at all pieces of this's colour on this's row
            std::shared_ptr<Piece> p = board[{i, posn.y}];
            if (p->getName() == (colour ? 'R' : 'r') && !p->getHasMoved() && // if p is a rook that hasn't moved yet and is at the same Y level as this
              posn.y == p->getY() && (posn.x - p->getX() == 4 || p->getX() - posn.x == 3)) { // and is either 4 spaces to the left or 3 to the right of this
                bool pathInCheck = false;
                for (int j = 1; j < (posn.x < p->getX() ? 3 : 4); j++) { // making sure there are no pieces in between and that
                    Posn pos{posn.x + (posn.x < p->getX() ? j : -j), posn.y}; // none of the positions in between are in check
                    if (board[pos] || board.check(pos, colour)) {
                        pathInCheck = true;
                        break;
                    }
                }
                if (!pathInCheck && !board.check(p->getPosn(), colour)) { // if none of the spaces in between are in check and the rook isn't in check either
                    legalMoves.emplace_back(posn.x + (posn.x < p->getX() ? 2 : -2), posn.y);
                }
            }
        }
    }
}

void King::calculatePins(const Board &board) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!i && !j) continue; // if i = j = 0, this means we are checking the square the king is currently on
            std::shared_ptr<Piece> allyFound = emptyptr;
            for (int k = 1;; k++) { // to scale in each direction
                try {
                    Posn p{posn.x + i * k, posn.y + j * k}; // will throw an exception if out of bounds, so I don't have to actually check
                    if (!board[p]) continue; // if there's nothing there, we don't care and move on
                    else if (board[p]->getColour() == colour) { // if the piece is on our team
                        if (allyFound) break; // if this is the second piece found on this axis, no pin will happen
                        else allyFound = board[p]; // if this is the first one found on this axis, remember where it is
                    } else if (allyFound && board[p]->getName() == (!colour ? 'Q' : 'q')) { // if it's a queen, it will pin no matter the axis
                        allyFound->pin(!i && j, i && !j, i == j, !(i + j));
                    } else if (allyFound && board[p]->getName() == (!colour ? 'R' : 'r')) { // if it's a rook
                        if ((!i && j) || (i && !j)) { // only pin if looking at vertical or horizontal
                            allyFound->pin(!i && j, i && !j, false, false);
                        } else break;
                    } else if (allyFound && board[p]->getName() == (!colour ? 'B' : 'b')) { // if it's a bishop
                        if (i && j) { // only pin if looking at diagonal axis
                            allyFound->pin(false, false, i == j, !(i + j));
                        } else break;
                    } else break; // if it's not a queen, bishop, or rook, no pin will happen
                } catch (BadPosn &e) {
                    break; // if p is out of bounds, break the k loop
                }
            }
        }
    }
}
