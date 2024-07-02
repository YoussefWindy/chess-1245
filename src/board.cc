// src/board.cc

#include "../include/board.h"
#include "../include/piece.h"

#include <iostream>
#include <string>
#include <memory>

using namespace std;

const unsigned int WIDTH = 8, HEIGHT = 8;

Board::Board() {
	// std::cout << "Created Board!" << std::endl;
}

Board::~Board() {
	// std::cout << "Destroyed Board!" << std::endl;
}

void Board::addPiece(char name, int posX, int posY) {
    board[posX][posY] = make_shared<Piece>(name, posX, posY);
}

void Board::addPiece(char name, string &pos) {
	unsigned int x, y;
	x = pos[0] - 'a';
	y = pos[1] - '1';
	if (0 <= x && x < WIDTH && 0 <= y && y < HEIGHT) {
		addPiece(name, x, y);
	}
}

const std::shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH] {
    return board;
}

std::ostream& operator<<(std::ostream& out, const Board& b) {
	// Get board data
	auto boardData = b.getBoard();
	
	// Iterate over the board
	for (int row = 0; row < HEIGHT; ++row) {
		// Row number
		out << (HEIGHT - row) << ' ';

		for (int col = 0; col < WIDTH; ++col) {
			// Get copy of piece data
			std::shared_ptr<Piece> piece = boardData[HEIGHT - row - 1][col];
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

	out << "  ";

	for (char c = 'a'; c < 'a' + WIDTH; ++c) {
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
