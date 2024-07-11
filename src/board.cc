// src/board.cc

#include "../include/board.h"

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
void Board::addPiece(const char name, const Posn &posn) {
	bool white = 'B' <= name && name <= 'R';
	char type = name - white ? ('A' - 'a') : 0;
	switch (type) {
		case 'p':
    		board[posn.x][posn.y] = make_shared<Pawn>(white, posn);
			break;
		case 'n':
    		board[posn.x][posn.y] = make_shared<Knight>(white, posn);
			break;
		case 'b':
    		board[posn.x][posn.y] = make_shared<Bishop>(white, posn);
			break;
		case 'r':
    		board[posn.x][posn.y] = make_shared<Rook>(white, posn);
			break;
		case 'q':
    		board[posn.x][posn.y] = make_shared<Queen>(white, posn);
			break;
		case 'k':
    		board[posn.x][posn.y] = make_shared<King>(white, posn);
			break;
	}
}

bool Board::movePiece(const Move &move) {
	if (board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos)) {
		board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y];
		removePiece(move.oldPos);
		return true;
	} else {
		return false;
	}
}

void Board::removePiece(const Posn &posn) {
	board[posn.x][posn.y] = emptyptr;
}

bool Board::positionInCheck(const Posn &posn, bool colour) const {
	for (auto piece: !colour ? whitePieces : blackPieces) {
		if (piece->canMoveTo(posn)) {
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
	return false;
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
	return true;
}

const shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH] {
    return board;
}

const std::shared_ptr<Piece> Board::operator[](const Posn &posn) const {
	return board[posn.x][posn.y];
}

ostream& operator<<(ostream& out, const Board& board) {
	// Iterate over the board
	for (unsigned int row = 0; row < HEIGHT; row++) {
		// Row number
		out << (HEIGHT - row) << ' ';
		for (unsigned int col = 0; col < WIDTH; col++) {
			// Get copy of piece data
			shared_ptr<Piece> piece = board[{HEIGHT - row - 1, col}];
			if (piece) {
				out << piece->getName();
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
	for (unsigned char c = 'a'; c < 'a' + WIDTH; c++) {
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
