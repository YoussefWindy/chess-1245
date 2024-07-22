// src/pieces/king.cc

#include "../../include/board.h"
#include "../../include/pieces/king.h"

King::King(bool colour, const Posn &posn):
  Piece{colour ? 'K' : 'k', colour, 10, posn, true, true, true, true} {}

void King::calculateLegalMoves(const Board &board) {
    // std::cerr << "King " << 1 << std::endl;
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
    // std::cerr << "King " << 2 << std::endl;
    if (!hasMoved && !board.check(posn, colour)) { // if king hasn't moved yet and is not in check
        for (unsigned int i = 0; i < WIDTH; i++) { // look at all pieces of this's colour on this's row
            std::shared_ptr<Piece> p = board[{i, posn.y}];
    // std::cerr << "King " << 3 << i << std::endl;
            if (p && p->getName() == (colour ? 'R' : 'r') && !p->getHasMoved() && // if p is a rook that hasn't moved yet and is at the same Y level as this
              posn.y == p->getY() && (posn.x - p->getX() == 4 || p->getX() - posn.x == 3)) { // and is either 4 spaces to the left or 3 to the right of this
                bool pathInCheck = false;
                for (int j = 1; j < (posn.x < p->getX() ? 3 : 4); j++) { // making sure there are no pieces in between and that
    // std::cerr << "King " << 4 << j << std::endl;
                    Posn pos{posn.x + (posn.x < p->getX() ? j : -j), posn.y}; // none of the positions in between are in check
                    if (board[pos] || board.check(pos, colour)) {
                        pathInCheck = true;
                        break;
                    }
                }
    // std::cerr << "King " << 5 << i << std::endl;
                if (!pathInCheck && !board.check(p->getPosn(), colour)) { // if none of the spaces in between are in check and the rook isn't in check either
                    legalMoves.emplace_back(posn.x + (posn.x < p->getX() ? 2 : -2), posn.y);
                }
            }
        }
    }
    // std::cerr << "King " << 6 << std::endl;
}

bool King::calculatePins(const Board &board, std::vector<Posn> &positions) {
    unsigned int numChecks = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!i && !j) continue; // if i = j = 0, this means we are checking the square the king is currently on
            std::shared_ptr<Piece> allyFound = emptyptr;
            std::vector<Posn> tmp;
            for (int k = 1;; k++) { // to scale in each direction
                try {
                    Posn p{posn.x + i * k, posn.y + j * k}; // will throw an exception if out of bounds, so I don't have to actually check
                    if (!board[p]) { // if there's nothing there, we note that square and move on
                        tmp.emplace_back(p);
                        continue;
                    } else if (board[p]->getColour() == colour) { // if we find an ally
                        if (allyFound) break; // if this is the second piece found on this axis, no pin will happen
                        else allyFound = board[p];
                        continue;
                    } else if (!allyFound) tmp.emplace_back(p);
                    if (board[p]->getName() == (!colour ? 'Q' : 'q')) { // if it's a queen, it will pin no matter the axis
                        if (allyFound) allyFound->pin(!i && j, i && !j, i == j, !(i + j));
                        else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'R' : 'r') && ((!i && j) || (i && !j))) { // if it's a rook and is looking vertical/horizontal
                        if (allyFound) allyFound->pin(!i && j, i && !j, false, false);
                        else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'B' : 'b') && i && j) { // if it's a bishop and is looking diagonally
                        if (allyFound) allyFound->pin(false, false, i == j, !(i + j));
                        else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'P' : 'p') && (colour ? i > 0 && j > 0 : i < 0 && j < 0)) {
                        if (numChecks) { // if there is already a threatening piece, two simultaneous checks means that
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions.emplace_back(p);
                            numChecks++;
                            break;
                        }
                    }
                } catch (BadPosn &e) {
                    break; // if p is out of bounds, break the k loop
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            unsigned int x = posn.x + (!(i % 2) ? i ? -2 : 2 : j ? -1 : 1);
            unsigned int y = posn.y + (i % 2 ? (i > 1 ? -2 : 2) : j ? -1 : 1);
            try {
                Posn p{x, y};
                if (board[p] && board[p]->getName() == (!colour ? 'N' : 'n')) {
                    if (numChecks) { // if there is already a threatening piece, two simultaneous checks means that
                        positions.clear(); // there is no way to block or capture both sources of check in one move
                        return true;
                    } else {
                        positions.emplace_back(p);
                        numChecks++;
                    }
                }
            } catch (BadPosn &e) {}
        }
    }
    return numChecks;
}
