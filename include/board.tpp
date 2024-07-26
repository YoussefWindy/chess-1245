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

void Board::promote(const Posn &posn, unsigned int type) {
	// std::cerr << "type is " << type << ". turn is " << (turn ? "white" : "black") << std::endl;
	removePiece(posn);
	// std::cerr << "this work" << std::endl;
	switch (type) {
		case 1:
			addPiece<Knight>(turn, posn);
		case 2:
			addPiece<Bishop>(turn, posn);
		case 3:
			addPiece<Rook>(turn, posn);
		case 4:
			addPiece<Queen>(turn, posn);
	}
	// std::cerr << char('a' + log.back().oldPos.x) << log.back().oldPos.y + 1
	//   << "-->" << char('a' + log.back().newPos.x) << log.back().newPos.y + 1 << " "
	//   << "capture: " << log.back().capture << ", promotion: " << log.back().promotion << " --> ";
	// std::cerr << log.back().promotion << std::endl;
}

#endif // BOARD_TPP
