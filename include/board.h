// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "../include/piece.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Board {
private:
	std::shared_ptr<Piece> board[WIDTH][HEIGHT];
public:
	Board();
	~Board();
	
	// Methods
	void addPiece(char name, int posX, int posY);
	void addPiece(char name, string &pos);

	const std::shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH];
};

std::ostream& operator<<(std::ostream& out, const Board& b);

#endif // BOARD_H
