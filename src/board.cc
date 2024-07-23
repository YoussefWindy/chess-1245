// src/board.cc

#include "../include/piece.h"
#include "../include/board.h"
#include "../include/board.tpp"
#include <iostream>
#include <ostream>

const Move emptyMove = {{0, 0}, {0, 0}};

Board::Board() : turn{true} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	whiteKing = blackKing = std::static_pointer_cast<King>(emptyptr);
}

Board::Board(const Board &other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces},
  deadPieces{other.deadPieces}, turn{other.turn}, log{other.log} {
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

Board::Board(Board &&other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces}, deadPieces{other.deadPieces},
  whiteKing{other.whiteKing}, blackKing{other.blackKing}, turn{other.turn}, log{other.log} {
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
	std::swap(one.turn, two.turn);
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

void Board::movePiece(const Move &move) {
	movePiece(std::move(move));
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
	// Invariant: this method should only ever be called if colour is in check, so we won't check for that
	for (auto piece: colour ? whitePieces : blackPieces) {
		std::cerr << "Checking " << piece->getName() << std::endl;
		if (piece->canMove()) return false;
	}
	return true;
}

bool Board::stalemate(bool colour) const {
	// Invariant: this method should only ever be called if colour isn't in check, so we won't check for that
	for (auto piece: !colour ? whitePieces : blackPieces) {
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
	board.movePiece(log[i]);
	i++;
	board.turn = !board.turn;
	return *this;
}

Board::Iterator& Board::Iterator::operator--() {
	if (!i) {
		throw BadMove{emptyMove};
	}
	board.undoMoves();
	i--;
	board.turn = !board.turn;
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

int Board::runCalculations() {
    std::vector<Posn> defensivePositions;
	std::cerr << 1 << std::endl;
	for (auto p: whitePieces) p->protect(false);
	for (auto p: blackPieces) p->protect(false);
	std::cerr << 2 << std::endl;
	std::cerr << whiteKing << std::endl << blackKing << std::endl;
	bool inCheck = (turn ? whiteKing : blackKing)->calculatePins(*this, defensivePositions);
	std::cerr << 2.5 << std::endl;
	(!turn ? whiteKing : blackKing)->calculatePins(*this, defensivePositions);
	std::cerr << 3 << std::endl;
	for (auto p: whitePieces) {
		std::cerr << p->getName() << " at " << char('a' + p->getX()) << p->getY() + 1 << ": ";
		if (p->getName() != 'K') {
			p->calculateLegalMoves(*this);
			std::cerr << "success";
			if (turn && inCheck) {
				std::cerr << " but we're checking here?";
				p->intersect(defensivePositions);
			}
			std::cerr << std::endl;
		}
	}
	std::cerr << 4 << std::endl;
	for (auto p: blackPieces) {
		if (p->getName() != 'k') {
			std::cerr << p->getName() << std::endl;
			p->calculateLegalMoves(*this);
			if (!turn && inCheck) {
				std::cerr << "what" << std::endl;
				p->intersect(defensivePositions);
			}
		}
	}
	std::cerr << 5 << std::endl;
	whiteKing->calculateLegalMoves(*this);
	std::cerr << "White king's moves: ";
	for (auto p: whiteKing->getLegalMoves()) std::cerr << char('a' + p.x) << p.y + 1 << ", ";
	blackKing->calculateLegalMoves(*this);
	std::cerr << std::endl << "Black king's moves: ";
	for (auto p: blackKing->getLegalMoves()) std::cerr << char('a' + p.x) << p.y + 1 << ", ";
	std::cerr << std::endl;
	if (inCheck) {
		if (checkmate(turn)) {
			std::cerr << "checkmate" << std::endl;
			return 2; // checkmate
		} else {
			std::cerr << "check" << std::endl;
			return 1; // check
		}
	} else if (stalemate(turn)) {
		std::cerr << "stalemate" << std::endl;
		return 0; // stalemate
	} else {
		std::cerr << "nothign" << std::endl;
		return -1; // nothing
	}
}

void Board::movePiece(Move &&move) {
	std::cerr << "start" << std::endl;
	if (!board[move.oldPos.x][move.oldPos.y]) {
		std::cout << "FIRST" << std::endl;
		throw BadMove{move};
  } else if (board[move.oldPos.x][move.oldPos.y]->getColour() != turn) {
		std::cout << "SECOND" << std::endl;
		throw BadMove{move};
  } else if (!board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos)) {
		auto testmoves = board[move.oldPos.x][move.oldPos.y]->getLegalMoves();
		std::cout << testmoves.size() << std::endl;
		for (auto i : testmoves) {
			std::cout << char('a' + i.x) << i.y + 1 << std::endl;
		}
		std::cout << "THIRD" << std::endl;
		throw BadMove{move};
	}
	std::cerr << "good" << std::endl;
	if (board[move.newPos.x][move.newPos.y]) { // if a capture is taking place
		// std::cerr << "capture" << std::endl;
		deadPieces.emplace_back(board[move.newPos.x][move.newPos.y]);
		removePiece(move.newPos);
		move.capture = true;
	}
	std::cerr << "up" << std::endl;
	(board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y])->move(move.newPos); // move the piece
	removePiece(move.oldPos);
	std::cerr << "down" << std::endl << "position is " << char('a' + move.newPos.x) << move.newPos.y + 1 << std::endl;
	std::cerr << "Piece at new position is " << board[move.newPos.x][move.newPos.y]->getName() << std::endl << "nice" << std::endl;
	if (board[move.newPos.x][move.newPos.y]->getName() == (turn ? 'K' : 'k')) { // check for castling
		std::cerr << "we're castling??" << std::endl;
		if (move.newPos.x - move.oldPos.x > 1) { // castling right
			std::cerr << char('a' + WIDTH - 1) << move.newPos.y + 1 << " --> " << char('a' + move.newPos.x - 1) << move.newPos.y + 1 << std::endl;
			movePiece({{WIDTH - 1, move.newPos.y}, {move.newPos.x - 1, move.newPos.y}}); // move the rook
			log.pop_back();
		} else if (move.oldPos.x - move.newPos.x > 1) { // castling left
			std::cerr << 'a' << move.newPos.y + 1 << " --> " << char('a' + move.newPos.x + 1) << move.newPos.y + 1 << std::endl;
			movePiece({{0, move.newPos.y}, {move.newPos.x + 1, move.newPos.y}}); // move the rook
			log.pop_back();
		}
	}
	log.emplace_back(move); // log move
	turn = !turn;
	std::cerr << "end" << std::endl;
}

void Board::removePiece(const Posn &posn) {
	if (!board[posn.x][posn.y]) return;
	std::cerr << "hmm" << std::endl;
	bool colour = board[posn.x][posn.y]->getColour();
	if (board[posn.x][posn.y]->getName() == (colour ? 'K' : 'k')) {
		(colour ? whiteKing : blackKing) = std::static_pointer_cast<King>(emptyptr);
	}
	std::cerr << "hhmmm" << std::endl;
	for (auto it = (colour ? whitePieces : blackPieces).begin(); it != (colour ? whitePieces : blackPieces).end(); it++) {
		if ((*it)->getPosn() == posn) {
			std::cerr << "hhhmmmm" << std::endl;
			(colour ? whitePieces : blackPieces).erase(it);
			break;
		} else if ((std::cerr << "bruh how" << std::endl) && (*it)->getName() == (colour ? 'K' : 'k')) {
			(colour ? whiteKing : blackKing) = std::static_pointer_cast<King>(*it);
		}
	}
	std::cerr << "???" << std::endl;
	board[posn.x][posn.y] = emptyptr;
	std::cerr << "ur kidding" << std::endl;
}

const std::shared_ptr<Piece> Board::operator[](const Posn &posn) const {
	return board[posn.x][posn.y];
}

Move Board::getLastMove() const {
	return log.empty() ? Move{{0, 0}, {0, 0}} : log.back();
}

void Board::undoMoves(int x) {
	for (int i = 0; i < x; i++) {
		if (log.empty()) return;
    	turn = !turn;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];
		board[log.back().oldPos.x][log.back().oldPos.y]->move(log.back().oldPos);
		removePiece(log.back().newPos);
		if (log.back().capture) {
			addPieceHelp(deadPieces.back()->getName(), deadPieces.back()->getPosn());
			(deadPieces.back()->getColour() ? whitePieces : blackPieces).emplace_back(deadPieces.back());
			deadPieces.pop_back();
		}
		log.pop_back();
	}
    runCalculations();
}

bool Board::getTurn() const {
  return turn;
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
	if (runCalculations() > 0) throw BadSetup{std::string((!turn ? "white" : "black")) + " is in check!"};
	turn = !turn;
	if (runCalculations() > 0) {
		turn = !turn;
		throw BadSetup{std::string((!turn ? "white" : "black")) + " is in check!"};
	}
	turn = !turn;
}

bool Board::hasKing(bool colour) const {
	return (colour ? whiteKing : blackKing) ? true : false;
}

void Board::setTurn(bool colour) {
	turn = colour;
}

const std::vector<Move> Board::getLog() const {
	return log;
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
