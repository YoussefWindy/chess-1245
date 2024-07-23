// include/board.tpp
#ifndef BOARD_TPP
#define BOARD_TPP

#include "board.h"

template <typename T>
void Board::addPiece(bool colour, const Posn &posn) {
	board[posn.x][posn.y] = std::make_shared<T>(colour, posn);
	(colour ? whitePieces : blackPieces).emplace_back(board[posn.x][posn.y]);
	if (std::is_same<T, King>::value) {
		(colour ? whiteKing : blackKing) = board[posn.x][posn.y];
		// if ((colour ? whiteKing : blackKing) == board[posn.x][posn.y]) {
		// 	std::cerr << "goodthing" << std::endl;
		// } else std::cerr << "badthing" << std::endl;
	}
}

void Board::promote(Move &&move, unsigned int type) {
	turn = !turn;
	// std::cerr << "type is " << type << ". turn is " << (turn ? "white" : "black") << std::endl;
	removePiece(move.newPos);
	// std::cerr << "this work" << std::endl;
	switch (type) {
		case 1:
			addPiece<Knight>(turn, {move.newPos.x, move.newPos.y});
		case 2:
			addPiece<Bishop>(turn, {move.newPos.x, move.newPos.y});
		case 3:
			addPiece<Rook>(turn, {move.newPos.x, move.newPos.y});
		case 4:
			addPiece<Queen>(turn, {move.newPos.x, move.newPos.y});
	}
	// std::cerr << char('a' + log.back().oldPos.x) << log.back().oldPos.y + 1
	//   << "-->" << char('a' + log.back().newPos.x) << log.back().newPos.y + 1 << " "
	//   << "capture: " << log.back().capture << ", promotion: " << log.back().promotion << " --> ";
	log.back().promotion = type; // note down the promotion type in the log
	// std::cerr << log.back().promotion << std::endl;
	turn = !turn;
}

#endif // BOARD_TPP
