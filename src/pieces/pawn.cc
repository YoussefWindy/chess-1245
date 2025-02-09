// src/pieces/pawn.cc

#include "../../include/board.h"
#include "../../include/pieces/pawn.h"

Pawn::Pawn(bool colour, const Posn &posn):
  Piece{colour ? 'P' : 'p', colour, 1, posn, true, false, true, true}, promotable{false} {}

void Pawn::calculateLegalMoves(const Board &board) {
    // std::cerr << "Pawn " << 1 << std::endl;
    legalMoves.clear();
    if (vertical && !board[{posn.x, posn.y + (colour ? 1 : -1)}]) { // if space in front is free
        // std::cerr << "Pawn " << 2 << ": " << char('a' + posn.x) << posn.y + (colour ? 1 : -1) + 1 << std::endl;
        legalMoves.emplace_back(posn.x, posn.y + (colour ? 1 : -1)); // can move forward one space
        // std::cerr << "SHOULD THE PAWN BE ABLE TO MOVE DOUBLE? " << !numMoves << std::endl;
        if ((colour ? posn.y == 1 : posn.y == HEIGHT - 2) && !numMoves && !board[{posn.x, (colour ? 3: HEIGHT - 4)}]) { // if two spaces ahead is free
            // std::cerr << "Pawn " << 3 << ": " << char('a' + posn.x) << (colour ? 3 : HEIGHT - 4) + 1 << std::endl;
            legalMoves.emplace_back(posn.x, colour ? 3 : HEIGHT - 4); // double movement if on starting space
        }
    }
    for (int i = 0; i < 2; i++) { // i = 0 looks to the right, i = 1 looks to the left
        if (!(i ? negative : positive)) continue;
        if ((i ? posn.x : posn.x < WIDTH - 1) && board[{posn.x + (i ? -1 : 1), posn.y + (colour ? 1 : -1)}]) {
            // std::cerr << "Pawn " << 4 << i << std::endl;
            if (board[{posn.x + (i ? -1 : 1), posn.y + (colour ? 1 : -1)}]->getColour() != colour) { // normal capture
                legalMoves.emplace_back(posn.x + (i ? -1 : 1), posn.y + (colour ? 1 : -1)); // can capture
            } else {
                board[{posn.x  + (i ? -1 : 1), posn.y + (colour ? 1 : -1)}]->protect(); // is protecting
            }
        } else if ((i ? posn.x : posn.x < WIDTH - 1) && posn.y == (colour ? HEIGHT - 4 : 3) && // en passant
            board[{posn.x + (i ? -1 : 1), colour ? HEIGHT - 4 : 3}] &&
            board[{posn.x + (i ? -1 : 1), colour ? HEIGHT - 4 : 3}]->getName() == (colour ? 'p' : 'P')
            && board.getLastMove() == Move{{posn.x + (i ? -1 : 1), colour ? HEIGHT - 2 : 1}, {posn.x  + (i ? -1 : 1), colour ? HEIGHT - 4 : 3}}) {
                // std::cerr << "Pawn " << 5 << i << std::endl;
                legalMoves.emplace_back(posn.x + (i ? -1 : 1), colour ? HEIGHT - 3 : 2);
        }
    }
    if (!legalMoves.empty()) promotable = (posn.y == (colour ? HEIGHT - 2 : 1));
    vertical = positive = negative = true;
}

bool Pawn::canPromote() const {
    return promotable;
}
