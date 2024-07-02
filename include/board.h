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
	std::vector<std::vector<std::shared_ptr<Piece>>> board;
public:
	Board();
	~Board();

	// Members
	const int size = 8;
	
	// Methods
	void addPiece(char name, int posX, int posY);
	const std::vector<std::vector<std::shared_ptr<Piece>>>& getBoard() const;
};

std::ostream& operator<<(std::ostream& out, const Board& b);

#endif // BOARD_H
