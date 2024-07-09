// src/board.cc

#include "../include/board.h"
#include "../include/piece.h"

#include <iostream>
#include <string>
#include <memory>

using namespace std;

Board::Board() {
	// cout << "Created Board!" << endl;
}

Board::~Board() {
	// cout << "Destroyed Board!" << endl;
}

Board::Iterator::Iterator(const shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin)
  : i{begin ? 0 : 8}, j{0}, board{board} {}

shared_ptr<Piece> Board::Iterator::operator*() const {
	return board[i][j];
}

Board::Iterator &Board::Iterator::operator++() {
	if (j == 7) {
		i++;
		j = 0;
	} else {
		j++;
	}
}

bool Board::Iterator::operator!=(const Iterator &other) const {
	return i != other.i || j != other.j;
}

Board::Iterator Board::begin() const {
	return {board, true};
}

Board::Iterator Board::end() const {
	return {board, false};
}

void Board::addPiece(const char name, const Posn pos) {
    board[pos.x][pos.y] = make_shared<Piece>(name, pos.x, pos.y);
}

void Board::removePiece(const Posn pos) {
	board[pos.x][pos.y] = nullptr;
}

bool Board::positionInCheck(Posn pos, bool colour) const {
	for (auto piece: !colour ? whitePieces : blackPieces) {
		if (piece->canReach(pos)) {
			return true;
		}
	}
	return false;
}

bool Board::checkmate(bool colour) const {
	for (auto piece: colour ? whitePieces : blackPieces) {
		if (piece->getName() == colour ? "K" : "k") {
			return !piece->canMove();
		}
	}
	// incomplete
}

bool Board::validate() const {
	bool white, black;
	for (shared_ptr<Piece> p: *this) {
		if (p->getName() == 'K') white = true;
	}
	if (!white) return false;
	for (shared_ptr<Piece> p: *this) {
		if (p->getName() == 'k') black = true;
	}
	if (!black) return false;
	// incomplete
}

const shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH] {
    return board;
}

ostream& operator<<(ostream& out, const Board& b) {
	// Get board data
	auto boardData = b.getBoard();
	
	// Iterate over the board
	for (int row = 0; row < HEIGHT; ++row) {
		// Row number
		out << (HEIGHT - row) << ' ';

		for (int col = 0; col < WIDTH; ++col) {
			// Get copy of piece data
			shared_ptr<Piece> piece = boardData[HEIGHT - row - 1][col];
			if (piece) {
				out << *piece;
			} else {
				if ((row + col) % 2) {
				out << '_';
				} else {
				out << ' ';
				}
			}
		}

		out << endl;
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
