// src/board.cc

#include "../include/board.h"
#include "../include/piece.h"

#include <iostream>
#include <string>
#include <memory>

Board::Board() {
	board.resize(8, std::vector<std::shared_ptr<Piece>>(8, nullptr));
}

Board::~Board() {
}

// Getter
const std::vector<std::vector<std::shared_ptr<Piece>>>& Board::getBoard() const {
	return board;
}

// Setter
void Board::addPiece(char name, int posX, int posY) {
    board[posX][posY] = std::make_shared<Piece>(name, posX, posY);
}

void Board::movePiece(int posX, int posY, int newX, int newY) {
	// Check if new position has a piece
	if (board[newX][newY] != nullptr &&
		board[posX][posY]->getColour() == board[newX][newY]->getColour()) {
			return;
	}

	// Set new position on board
	board[newX][newY] = board[posX][posY];
	
	// Clear old position
	board[posX][posY] = nullptr;
	
	// Update piece data
	board[newX][newY]->setX(newX);
	board[newX][newY]->setY(newY);
}

// IO
std::ostream& operator<<(std::ostream& out, const Board& b) {
	// Get board data
	auto boardData = b.getBoard();
	
	// Iterate over the board
	for (int row = 0; row < b.size; ++row) {
		// Row number
		out << (8 - row) << ' ';

		for (int col = 0; col < b.size; ++col) {
			// Get copy of piece data
			std::shared_ptr<Piece> piece = boardData[b.size - row - 1][col];
			if (piece) {
				out << *piece;
			} else {
				if (col % 2 == 0) {
				out << '_';
				} else {
				out << ' ';
				}
			}
		}

		out << std::endl;
	}

	out << std::endl << "  ";

	for (char c = 'a'; c < 'a' + b.size; ++c) {
		out << c;
	}

	return out;	
}

/*

8 _ _n_ _ 
7 _ _ _ _ 
6 _ _ _ _ 
5 _ _ _ _ 
4 _ _ _ _ 
3 _ _ _ _ 
2 _ _ _ _ 
1 b _ _ _ 

  abcdefgh

*/
