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
	// 2D array of smart pointers to Piece objects
	std::vector<std::vector<std::shared_ptr<Piece>>> board;
public:
	Board();
	~Board();

	// Members
	const int size = 8;
	
	// Getter
	const std::vector<std::vector<std::shared_ptr<Piece>>>& getBoard() const;

	
	// Setting
	void addPiece(char name, int posX, int posY);
	void movePiece(int posX, int posY, int newX, int newY);
};

// IO
std::ostream& operator<<(std::ostream& out, const Board& b);

#endif // BOARD_H
