// src/board.cc

#include "../include/piece.h"
#include "../include/board.h"

Board::Board() {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	whiteKing = blackKing = std::static_pointer_cast<King>(emptyptr);
	// cout << "Created Board!" << endl;
}

Board::Board(const Board &other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces},
  deadPieces{other.deadPieces}, whiteKing{other.whiteKing}, blackKing{other.blackKing}, log{other.log} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = other.board[i][j];
		}
	}
}

Board::Board(Board &&other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces},
  deadPieces{other.deadPieces}, whiteKing{other.whiteKing}, blackKing{other.blackKing}, log{other.log} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = move(other.board[i][j]);
		}
	}
}

void Board::swap(Board &one, Board &two) {
	std::swap(one.board, two.board);
	std::swap(one.whitePieces, two.whitePieces);
	std::swap(one.blackPieces, two.blackPieces);
	std::swap(one.deadPieces, two.deadPieces);
	std::swap(one.whiteKing, two.whiteKing);
	std::swap(one.blackKing, two.blackKing);
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

bool Board::check(const Posn &posn, bool colour) const {
	for (auto piece: !colour ? whitePieces : blackPieces) {
		if (piece->canMoveTo(posn)) {
			return true;
		}
	}
	return false;
}

bool Board::checkmate(bool colour) const {
	if ((colour ? whiteKing : blackKing)->canMove()) return false;
	// incomplete
	return true;
}

bool Board::stalemate(bool colour) const {
	// Invariant: this method should only ever be called if colour isn't in check, so we won't check for that
	for (auto piece: colour ? whitePieces : blackPieces) {
		if (piece->canMove()) {
			return false;
		}
	}
	return true;
}

Board::Iterator::Iterator(const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin):
  i{begin ? 0 : WIDTH}, j{0}, board{board} {}

std::shared_ptr<Piece> Board::Iterator::operator*() const {
	return board[i][j];
}

Board::Iterator& Board::Iterator::operator++() {
	if (j == HEIGHT - 1) {
		i++;
		j = 0;
	} else {
		j++;
	}
	return *this;
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

int Board::runCalculations(bool colour) {
	whiteKing->calculatePins(*this);
	blackKing->calculatePins(*this);
	for (auto p: whitePieces) {
		if (p->getName() != 'K') {
			p->calculateLegalMoves(*this);
		}
	}
	for (auto p: blackPieces) {
		if (p->getName() != 'k') {
			p->calculateLegalMoves(*this);
		}
	}
	whiteKing->calculateLegalMoves(*this);
	blackKing->calculateLegalMoves(*this);
	if (check((colour ? whiteKing : blackKing)->getPosn(), colour)) {
		if (checkmate(colour)) {
			return 2; // checkmate
		} else {
			return 1; // check
		}
	} else if (stalemate(colour)) {
		return 0; // stalemate
	} else {
		return -1; // nothing
	}
}

void Board::movePiece(Move &&move) {
	if (!(board[move.oldPos.x][move.oldPos.y] && board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos))) {
		throw BadMove{move};
	}
	if (board[move.newPos.x][move.newPos.y]) { // if a capture is taking place
		deadPieces.emplace_back(board[move.newPos.x][move.newPos.y]);
		removePiece(move.newPos);
	}
	board[move.oldPos.x][move.oldPos.y]->move(move.newPos); // update the piece's internal posn
	board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y]; // move the piece
	removePiece(move.oldPos);
	if (board[move.newPos.x][move.newPos.y]->getName() == (board[move.newPos.x][move.newPos.y]->getColour() ? 'K' : 'k')) { // check for castling
		if (move.newPos.x - move.oldPos.x > 1) { // castling right
			movePiece({{WIDTH - 1, move.newPos.y}, {move.newPos.x - 1, move.newPos.y}}); // move the rook
		} else if (move.oldPos.x - move.newPos.x > 1) { // castling left
			movePiece({{0, move.newPos.y}, {move.newPos.x + 1, move.newPos.y}}); // move the rook
		}
	}
	log.emplace_back(move); // log move
}

void Board::removePiece(const Posn &posn) {
	if (!board[posn.x][posn.y]) return;
	bool colour = board[posn.x][posn.y]->getColour();
	if (board[posn.x][posn.y]->getName() == (colour ? 'K' : 'k')) {
		colour ? whiteKing : blackKing = std::static_pointer_cast<King>(emptyptr);
	}
	for (auto it = (colour ? whitePieces : blackPieces).begin(); it != (colour ? whitePieces : blackPieces).end(); it++) {
		if (it->get()->getPosn() == posn) {
			(colour ? whitePieces : blackPieces).erase(it);
			break;
		}
	}
	board[posn.x][posn.y] = emptyptr;
}

const std::shared_ptr<Piece> Board::operator[](const Posn &posn) const {
	return board[posn.x][posn.y];
}

Move Board::getLastMove() const {
	return log.empty() ? Move{{0, 0}, {0, 0}} : log.back();
}

bool Board::undoMoves(int x) {
	for (int i = 0; i < x; i++) {
		if (log.empty()) return false;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];
		removePiece(log.back().newPos);
		if (deadPieces.back()->getPosn() == log.back().newPos) {
			switch (deadPieces.back()->getName()) {
				case 'p':
					addPiece<Pawn>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
				case 'n':
					addPiece<Knight>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
				case 'b':
					addPiece<Bishop>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
				case 'r':
					addPiece<Rook>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
				case 'q':
					addPiece<Queen>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
				case 'k':
					addPiece<King>(deadPieces.back()->getColour(), deadPieces.back()->getPosn());
					break;
			}
			(deadPieces.back()->getColour() ? whitePieces : blackPieces).emplace_back(deadPieces.back());
			deadPieces.pop_back();
		}
		log.pop_back();
	}
	return true;
}

bool Board::validate() const {
	// Check number of w/b kings == 1 each
	int numWhiteKings = 0;
	int numBlackKings = 0;

	// Check white pieces
	for (auto p: whitePieces) {
		if (p->getName() == 'K') {
			++numWhiteKings;
		}

		if (p->getName() == 'P') {
			Posn pawnPosn = p->getPosn();
			if (pawnPosn.x == 7) return false;
		}
	}
	if (numWhiteKings != 1) return false;

	// Check black pieces
	for (auto p: blackPieces) {
		if (p->getName() == 'k') {
			++numBlackKings;
		}

		if (p->getName() == 'p') {
			Posn pawnPosn = p->getPosn();
			if (pawnPosn.x == 0) return false;
		}
	}
	if (numBlackKings != 1) return false;

	return true;
}

bool Board::hasKing(bool colour) const {
	return (colour ? whiteKing : blackKing) ? true : false;
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	for (unsigned int row = 0; row < HEIGHT; row++) { // Iterate over the board
		out << (HEIGHT - row) << "  "; // Row number
		for (unsigned int col = 0; col < WIDTH; col++) {
			if (board[{HEIGHT - row - 1, col}]) { // if there's a piece there
				out << board[{HEIGHT - row - 1, col}]->getName() << ((row + col) % 2 ? ' ' : '_');
			} else { // if it's a blank space
				out << ((row + col) % 2 ? "  " : "__");
			}
		}
		out << std::endl << std::endl;
	}
	out << "  ";
	for (unsigned char c = 'a'; c < 'a' + WIDTH; c++) {
		out << c << ' ';
	}
	out << std::endl;
	return out;
}

/*
	8   __  __n __  __
	7 __  __  __  __  
	6   __  __  __  __
	5 __  __  __  __  
	4   __  __  __  __
	3 __  __  __  __  
	2   __  __  __  __
	1 __b __  __  __  

	  a b c d e f g h
*/
