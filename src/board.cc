// src/board.cc

#include "../include/piece.h"
#include "../include/board.h"
#include "../include/board.tpp"
#include <iostream>
#include <ostream>

const Move emptyMove = {{0, 0}, {0, 0}};

Board::Board() : whiteTurn{true}{
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	whiteKing = blackKing = std::static_pointer_cast<King>(emptyptr);
}

Board::Board(const Board &other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces},
  deadPieces{other.deadPieces}, log{other.log} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	for (auto p: other.whitePieces) {
		addPieceHelp(p->getName(), p->getPosn());
	}
	for (auto p: other.blackPieces) {
		addPieceHelp(p->getName(), p->getPosn());
	}
	for (auto p: other.deadPieces) {
		deadPieces.emplace_back(p); // <-- LAZY! FIX THIS
	}
}

Board::Board(Board &&other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces},
  deadPieces{other.deadPieces}, whiteKing{other.whiteKing}, blackKing{other.blackKing}, log{other.log} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = other.board[i][j];
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

void Board::addPieceHelp(char name, const Posn &posn) {
	bool white = 'B' <= name && name <= 'R';
	name -= (white ? ('A' - 'a') : 0);
	switch (name) {
		case 'p':
			addPiece<Pawn>(white, posn);
			break;
		case 'n':
			addPiece<Knight>(white, posn);
			break;
		case 'b':
			addPiece<Bishop>(white, posn);
			break;
		case 'r':
			addPiece<Rook>(white, posn);
			break;
		case 'q':
			addPiece<Queen>(white, posn);
			break;
		case 'k':
			addPiece<King>(white, posn);
			break;
	}
}

void Board::movePiece(bool colour, const Move &move) {
	movePiece(colour, std::move(move));
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
	for (auto piece: colour ? whitePieces : blackPieces) {
		if (piece->canMove()) return false;
	}
	return true;
}

bool Board::stalemate(bool colour) const {
	// Invariant: this method should only ever be called if colour isn't in check, so we won't check for that
	for (auto piece: colour ? whitePieces : blackPieces) {
		if (piece->canMove()) return false;
	}
	if (log.size() < 6) return false;
	// TODO: handle 3 same moves in a row thing
	return true;
}

Board::Iterator::Iterator(const Board &board, const std::vector<Move> &log, bool begin):
  i{begin ? 0 : log.size() - 1}, board(const_cast<Board&>(board)), log{log} {}

const std::shared_ptr<Piece> (&Board::Iterator::operator*() const)[WIDTH][HEIGHT] {
	return board.board;
}

Board::Iterator& Board::Iterator::operator++() {
	if (i == log.size() - 1) {
		throw BadMove{emptyMove};
	}
	board.movePiece(board.whiteTurn, log[i]);
	i++;
	board.whiteTurn = !board.whiteTurn;
	return *this;
}

Board::Iterator& Board::Iterator::operator--() {
	if (!i) {
		throw BadMove{emptyMove};
	}
	board.undoMoves();
	i--;
	board.whiteTurn = !board.whiteTurn;
	return *this;
}

bool Board::Iterator::operator!=(const Iterator &other) const {
	return i != other.i;
}

Board::Iterator Board::begin() const {
	return {*this, log, true};
}

Board::Iterator Board::end() const {
	return {*this, log, false};
}

int Board::runCalculations(bool colour) {
    std::vector<Posn> defensivePositions;
	// std::cerr << 1 << std::endl;
	for (auto p: whitePieces) p->protect(false);
	for (auto p: blackPieces) p->protect(false);
	// std::cerr << 2 << std::endl;
	bool inCheck = (colour ? whiteKing : blackKing)->calculatePins(*this, defensivePositions);
	(!colour ? whiteKing : blackKing)->calculatePins(*this, defensivePositions);
	// std::cerr << 3 << std::endl;
	for (auto p: whitePieces) {
		if (p->getName() != 'K') {
			p->calculateLegalMoves(*this);
			if (colour && inCheck) {
				p->intersect(defensivePositions);
			}
		}
	}
	// std::cerr << 4 << std::endl;
	for (auto p: blackPieces) {
		if (p->getName() != 'k') {
	// std::cerr << p->getName() << std::endl;
			p->calculateLegalMoves(*this);
			if (!colour && inCheck) {
	// std::cerr << "what" << std::endl;
				p->intersect(defensivePositions);
			}
		}
	}
	// std::cerr << 5 << std::endl;
	whiteKing->calculateLegalMoves(*this);
	blackKing->calculateLegalMoves(*this);
	// std::cerr << 6 << std::endl;
	if (inCheck) {
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
	// std::cerr << 7 << std::endl;
}

void Board::movePiece(bool colour, Move &&move) {
	// std::cerr << "start" << std::endl;
	if (!board[move.oldPos.x][move.oldPos.y]) {
    std::cout << "FIRST" << std::endl;
    throw BadMove{move};
  } else if (board[move.oldPos.x][move.oldPos.y]->getColour() != colour) {
    std::cout << "SECOND" << std::endl;
    throw BadMove{move};
  } else if (!board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos)) {
    auto testmoves = board[move.oldPos.x][move.oldPos.y]->getLegalMoves();
    std::cout << testmoves.size() << std::endl;
    for (auto i : testmoves) {
      std::cout << "move.oldPos = (" << move.oldPos.x << ", " << move.oldPos.y << ')' << std::endl;
      std::cout << "move.newPos = (" << move.newPos.x << ", " << move.newPos.y << ')' << std::endl;
      std::cout << "Posn i = (" << i.x << ", " << i.y << ')' << std::endl;
    }
    std::cout << "THIRD" << std::endl;
    throw BadMove{move};
	}
	// std::cerr << "good" << std::endl;
	if (board[move.newPos.x][move.newPos.y]) { // if a capture is taking place
		// std::cerr << "capture" << std::endl;
		deadPieces.emplace_back(board[move.newPos.x][move.newPos.y]);
		removePiece(move.newPos);
		move.capture = true;
	}
	// std::cerr << "up" << std::endl;
	(board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y])->move(move.newPos); // move the piece
	removePiece(move.oldPos);
	// std::cerr << "down" << std::endl;
	if (board[move.newPos.x][move.newPos.y]->getName() == (board[move.newPos.x][move.newPos.y]->getColour() ? 'K' : 'k')) { // check for castling
		// std::cerr << "we're castling??" << std::endl;
		if (move.newPos.x - move.oldPos.x > 1) { // castling right
			movePiece(colour, {{WIDTH - 1, move.newPos.y}, {move.newPos.x - 1, move.newPos.y}}); // move the rook
		} else if (move.oldPos.x - move.newPos.x > 1) { // castling left
			movePiece(colour, {{0, move.newPos.y}, {move.newPos.x + 1, move.newPos.y}}); // move the rook
		}
	}
	
  log.emplace_back(move); // log move

  whiteTurn = !whiteTurn;
	// std::cerr << "end" << std::endl;
}

void Board::removePiece(const Posn &posn) {
	if (!board[posn.x][posn.y]) return;
	bool colour = board[posn.x][posn.y]->getColour();
	if (board[posn.x][posn.y]->getName() == (colour ? 'K' : 'k')) {
		(colour ? whiteKing : blackKing) = std::static_pointer_cast<King>(emptyptr);
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
    whiteTurn = !whiteTurn;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];
		removePiece(log.back().newPos);
		if (deadPieces.size() != 0 && deadPieces.back()->getPosn() == log.back().newPos) {
			addPieceHelp(deadPieces.back()->getName(), deadPieces.back()->getPosn());
			(deadPieces.back()->getColour() ? whitePieces : blackPieces).emplace_back(deadPieces.back());
			deadPieces.pop_back();
		}
		log.pop_back();

    runCalculations(whiteTurn);
	}

	return true;
}

bool Board::getTurn() const {
  return this->whiteTurn;
}

void Board::validate() {
	if (!hasKing(true)) throw BadSetup{"no white king on the board!"};
	if (!hasKing(false)) throw BadSetup{"no black king on the board!"};
	for (auto p: whitePieces) { // Check white pawns
		if (p->getName() == 'P') {
			Posn pawnPosn = p->getPosn();
			if (pawnPosn.y == HEIGHT - 1) throw BadSetup{"there is a white pawn on the last row!"};
		}
	}
	for (auto p: blackPieces) { // Check black pawns
		if (p->getName() == 'p') {
			Posn pawnPosn = p->getPosn();
			if (!pawnPosn.y) throw BadSetup{"there is a black pawn on the last row!"};
		}
	}
	if (runCalculations(true) > 0) throw BadSetup{"white is in check!"};
	if (runCalculations(false) > 0) throw BadSetup{"black is in check!"};
}

bool Board::hasKing(bool colour) const {
	for (auto p : (colour ? whitePieces : blackPieces)) {
		if (p->getName() == 'k' || p->getName() == 'K') return true;
	}

	return false;
	// return (colour ? whiteKing : blackKing) ? true : false;
}

const std::vector<Move> Board::getLog() const {
	return this->log;
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	out << std::endl;
	for (unsigned int row = 0; row < HEIGHT; row++) { // Iterate over the board
		out << (HEIGHT - row) << " "; // Row number
		for (unsigned int col = 0; col < WIDTH; col++) {
			if (board[{col, HEIGHT - row - 1}]) { // if there's a piece there
				out << board[{col, HEIGHT - row - 1}]->getName();
			} else { // if it's a blank space
				out << ((col + row) % 2 ? '_' : ' ');
			}
		}
		if (row == 1) {
			out << "   ";
			for (auto p: board.deadPieces) {
				if (p->getColour()) {
					out << p->getName();
				}
			}
		} else if (row == HEIGHT - 2) {
			out << "   ";
			for (auto p: board.deadPieces) {
				if (!p->getColour()) {
					out << p->getName();
				}
			}
		}
		out << std::endl;
	}
	out << std::endl << "  ";
	for (unsigned char c = 'a'; c < 'a' + WIDTH; c++) {
		out << c;
	}
	out << std::endl;

	return out;
}
