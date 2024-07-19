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

void Board::promote(bool colour, Move &&move, unsigned int type) {
	deadPieces.emplace_back(board[move.newPos.x][move.newPos.y]);
	for (auto it = (colour ? whitePieces : blackPieces).begin(); it != (colour ? whitePieces : blackPieces).end(); it++) {
		if ((*it)->getPosn() == move.newPos) {
			(colour ? whitePieces : blackPieces).erase(it);
			break;
		}
	}
	switch (type) {
		case 1:
			addPiece<Knight>(colour, {move.newPos.x, move.newPos.y + (colour ? 1 : -1)});
		case 2:
			addPiece<Bishop>(colour, {move.newPos.x, move.newPos.y + (colour ? 1 : -1)});
		case 3:
			addPiece<Rook>(colour, {move.newPos.x, move.newPos.y + (colour ? 1 : -1)});
		case 4:
			addPiece<Queen>(colour, {move.newPos.x, move.newPos.y + (colour ? 1 : -1)});
	}
	log.back().promotion = type;
}

#endif // BOARD_TPP
