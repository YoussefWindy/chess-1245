// include/board.tpp
#ifndef BOARD_TPP
#define BOARD_TPP

#include "board.h"

template <typename T>
void Board::addPiece(bool colour, const Posn &posn) {
	board[posn.x][posn.y] = std::make_shared<T>(colour, posn);
	(colour ? whitePieces : blackPieces).emplace_back(board[posn.x][posn.y]);
	if (std::is_same<T, King>::value) {
		(colour ? whiteKing : blackKing) = std::static_pointer_cast<King>(board[posn.x][posn.y]);
	}
}

template <typename T>
void Board::promote(bool colour, const Posn &posn) {
	deadPieces.emplace_back(board[posn.x][posn.y]);
	if (colour) {
		for (auto it = whitePieces.begin(); it != whitePieces.end(); it++) {
			if (it->get()->getPosn() == posn) {
				whitePieces.erase(it);
				break;
			}
		}
	} else {
		for (auto it = blackPieces.begin(); it != blackPieces.end(); it++) {
			if (it->get()->getPosn() == posn) {
				blackPieces.erase(it);
				break;
			}
		}
	}
	addPiece<T>(colour, {posn.x, posn.y + (colour ? 1 : -1)});
}

#endif // BOARD_TPP
