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
                if ((!board[p] || (board[p]->getColour() != colour && // if square is empty, or square is not belonging to us already and is not
                  !board[p]->getIsProtected())) && !board.check(p, colour) // touching a king and not is protected, and square is not in check, and
                  && (p.y == (colour ? HEIGHT - 1 : 0) || ((p.x == WIDTH - 1 || !(board[{p.x + 1, p.y + (colour ? 1 : -1)}]) // there are no
                  || (board[{p.x + 1, p.y + (colour ? 1 : -1)}]->getName() != (!colour ? 'P' : 'p'))) && (!p.x || // pawns that can check
                  !(board[{p.x - 1, p.y + (colour ? 1 : -1)}]) || (board[{p.x - 1, p.y + (colour ? 1 : -1)}]->getName() != (!colour ? 'P' : 'p')))))
                  && (abs(p.x - (!colour ? board.whiteKing : board.blackKing)->getX()) > 1 || abs(p.y - (!colour ? board.whiteKing : board.blackKing)->getY()) > 1)) {
                    // std::cerr << "King can move to " << char('a' + p.x) << p.y + 1 << std::endl;
                    legalMoves.emplace_back(p);
                }
            } catch (BadPosn &e) {}
        }
    }
    // std::cerr << "King " << 2 << std::endl;
    if (!numMoves && !board.check(posn, colour)) { // if king hasn't moved yet and is not in check
        for (int i = 0; i < 2; i++) { // look at the two rooks on either side of the board
            std::shared_ptr<Piece> piece = board[{i ? 0 : WIDTH - 1, posn.y}];
    // std::cerr << "King " << 3 << i << std::endl;
            if (piece && piece->getName() == (colour ? 'R' : 'r') && !piece->getHasMoved() && // if p is a rook that hasn't moved yet and is at the same Y level as this
              posn.y == piece->getY() && (posn.x - piece->getX() == 4 || piece->getX() - posn.x == 3)) { // and is either 4 spaces to the left or 3 to the right of this
                if (!board[{posn.x + (i ? -1 : 1), posn.y}] && !board.check({posn.x + (i ? -1 : 1), posn.y}, colour) &&
                  !board[{posn.x + (i ? -2 : 2), posn.y}] && !board.check({posn.x + (i ? -2 : 2), posn.y}, colour) && (!i || !board[{posn.x - 3, posn.y}])) {
                    legalMoves.emplace_back(posn.x + (i ? -2 : 2), posn.y); // if none of the spaces in between are in check
                }
            }
        }
    }
    // std::cerr << "King " << 5 << std::endl;
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
                        // std::cerr << "1a Nothing at " << char('a' + p.x) << p.y + 1 << std::endl;
                        tmp.emplace_back(p);
                        continue;
                    } else if (board[p]->getColour() == colour) { // if we find an ally
                        // std::cerr << "1b Ally at " << char('a' + p.x) << p.y + 1 << ". allyFound: " << allyFound << std::endl;
                        if (allyFound) break; // if this is the second piece found on this axis, no pin will happen
                        else allyFound = board[p];
                        continue;
                    } else if (!allyFound) tmp.emplace_back(p);
                    if (board[p]->getName() == (!colour ? 'Q' : 'q')) { // if it's a queen, it will pin no matter the axis
                        // std::cerr << "2a Queen at " << char('a' + p.x) << p.y + 1 << ". allyFound: " << allyFound << ". numChecks: " << numChecks << std::endl;
                        if (allyFound) {
                            allyFound->pin(!i && j, i && !j, i == j, !(i + j));
                            break;
                        } else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'R' : 'r')) { // if it's a rook and is looking vertical/horizontal
                        if ((i || !j) && (!i || j)) {
                            // std::cerr << "done" << std::endl;
                            break;
                        }
                        // std::cerr << "2b Rook at " << char('a' + p.x) << p.y + 1<< ". allyFound: " << allyFound << ". numChecks: " << numChecks << std::endl;
                        if (allyFound) {
                            allyFound->pin(!i && j, i && !j, false, false);
                            break;
                        } else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'B' : 'b')) { // if it's a bishop and is looking diagonally
                        if (!i || !j) {
                            // std::cerr << "done" << std::endl;
                            break;
                        }
                        // std::cerr << "2c Bishop at " << char('a' + p.x) << p.y + 1 << ". allyFound: " << allyFound << ". numChecks: " << numChecks << std::endl;
                        if (allyFound) {
                            allyFound->pin(false, false, i == j, !(i + j));
                            break;
                        } else if (numChecks) { // if there is already a threatening piece, two simultaneous checks means
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions = tmp;
                            numChecks++;
                            break;
                        }
                    } else if (board[p]->getName() == (!colour ? 'P' : 'p')) {
                        // std::cerr << "2d Pawn at " << p.x << ", " << p.y << ". allyFound: " << allyFound << ". numChecks: " << numChecks << std::endl;
                        if (k != 1 || !(colour ? i > 0 && j > 0 : i < 0 && j < 0)) {
                            // std::cerr << "done" << std::endl;
                            break;
                        }
                        if (numChecks) { // if there is already a threatening piece, two simultaneous checks means that
                            positions.clear(); // there is no way to block or capture both sources of check in one move
                            return true;
                        } else {
                            positions.emplace_back(p);
                            numChecks++;
                            break;
                        }
                    } else break; // only other piece types it could be are king and knight, which will block checks
                } catch (BadPosn &e) {
                    break; // if p is out of bounds, break the k loop to start looking in a different direction
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
                    // std::cerr << "2d Pawn at " << char('a' + p.x) << p.y << ". numChecks: " << numChecks << std::endl;
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
    // std::cerr << numChecks << std::endl;
    return numChecks;
}
