// include/board.tpp
#ifndef BOARD_TPP
#define BOARD_TPP

#include "board.h"
#include <memory>

template <typename T>
void Board::addPiece(bool colour, const Posn &posn) {
	board[posn.x][posn.y] = std::make_shared<T>(colour, posn);
}

#endif // BOARD_TPP
