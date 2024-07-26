// include/board.tpp
#ifndef BOARD_TPP
#define BOARD_TPP

#include "board.h"

template <typename T>
void Board::addPiece(bool colour, const Posn &posn) {
	board[posn.x][posn.y] = std::make_shared<T>(colour, posn);
	insert(colour ? whitePieces : blackPieces, board[posn.x][posn.y]);
	if (std::is_same<T, King>::value) {
		(colour ? whiteKing : blackKing) = board[posn.x][posn.y];
		// if ((colour ? whiteKing : blackKing) == board[posn.x][posn.y]) {
		// 	std::cerr << "goodthing" << std::endl;
		// } else std::cerr << "badthing" << std::endl;
	}
}

#endif // BOARD_TPP
