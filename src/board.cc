// src/board.cc

#include "../include/board.h"

using namespace std;

Board::Board() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	// cout << "Created Board!" << endl;
}

Board::Board(const Board &other): whitePieces{other.whitePieces},
  blackPieces{other.blackPieces}, deadPieces{other.deadPieces}, log{other.log} {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			board[i][j] = other.board[i][j];
		}
	}
}

Board::Board(Board &&other): whitePieces{other.whitePieces},
  blackPieces{other.blackPieces}, deadPieces{other.deadPieces}, log{other.log} {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			board[i][j] = move(other.board[i][j]);
		}
	}
}

void Board::swap(Board &one, Board &two) {
	std::swap(one.board, two.board);
	std::swap(one.whitePieces, two.whitePieces);
	std::swap(one.blackPieces, two.blackPieces);
	std::swap(one.deadPieces, two.deadPieces);
	std::swap(one.log, two.log);
}

Board& Board::operator=(const Board &other) {
	Board tmp = other;
	swap(*this, tmp);
	return *this;
}

Board& Board::operator=(Board &&other) {
	swap(*this, other);
	return *this;
}

Board::~Board() {
	// cout << "Destroyed Board!" << endl;
}

Board::Iterator::Iterator(const shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin):
  i{begin ? 0 : 8}, j{0}, board{board} {}

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

void Board::addPawn(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<Pawn>(colour, posn);
}

void Board::addKnight(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<Knight>(colour, posn);
}

void Board::addBishop(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<Bishop>(colour, posn);
}

void Board::addRook(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<Rook>(colour, posn);
}

void Board::addQueen(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<Queen>(colour, posn);
}

void Board::addKing(bool colour, const Posn &posn) {
    board[posn.x][posn.y] = make_shared<King>(colour, posn);
}

void Board::movePiece(Move &&move) {
	if (!board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos)) {
		throw BadMove{move};
	}
	board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y];
	removePiece(move.oldPos);
	log.emplace_back(move);
}

void Board::removePiece(const Posn &posn) {
	board[posn.x][posn.y] = emptyptr;
}

const shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH] {
    return board;
}

const std::shared_ptr<Piece> Board::operator[](const Posn &posn) const {
	return board[posn.x][posn.y];
}

Move Board::getLastMove() const {
	return log.back();
}

void Board::undoMoves(int x) {
	for (int i = 0; i < x; i++) {
		if (log.empty()) break;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];
		removePiece(log.back().newPos);
		log.pop_back();
	}
}

bool Board::inCheck(const Posn &posn, bool colour) const {
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
