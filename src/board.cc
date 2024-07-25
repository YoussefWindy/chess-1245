// src/board.cc

#include "../include/piece.h"
#include "../include/board.h"
#include "../include/board.tpp"
#include <iostream>
#include <ostream>

const Move emptyMove = {{0, 0}, {0, 0}};

Board::Board() : turn{true}, showDead{false} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	whiteKing = blackKing = emptyptr;
}

Board::Board(const Board &other): turn{other.turn}, showDead{other.showDead}, log{other.log} {
	for (unsigned int i = 0; i < WIDTH; i++) {
		for (unsigned int j = 0; j < HEIGHT; j++) {
			board[i][j] = emptyptr;
		}
	}
	for (auto p: other.deadPieces) {
		addPieceHelp(p->getName(), p->getPosn());
		insert(deadPieces, board[p->getX()][p->getY()]);
		removePiece(p->getPosn());
	}
	for (auto p: other.whitePieces) {
		addPieceHelp(p->getName(), p->getPosn());
		if (p->getName() == 'K') {
			whiteKing = board[p->getX()][p->getY()];
		}
	}
	for (auto p: other.blackPieces) {
		addPieceHelp(p->getName(), p->getPosn());
		if (p->getName() == 'k') {
			blackKing = board[p->getX()][p->getY()];
		}
	}
}

Board::Board(Board &&other): whitePieces{other.whitePieces}, blackPieces{other.blackPieces}, deadPieces{other.deadPieces},
  whiteKing{other.whiteKing}, blackKing{other.blackKing}, turn{other.turn}, showDead{other.showDead}, log{other.log} {
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
	std::swap(one.showDead, two.showDead);
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

void Board::insert(std::vector<std::shared_ptr<Piece>> &vec, std::shared_ptr<Piece> &piece) {
	for (auto it = vec.begin();; it++) {
		if (it == vec.end() || ((**it) <=> (*piece)) > 0) {
			vec.emplace(it, piece);
			break;
		}
	}
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
		// std::cerr << "Checking " << piece->getName() << std::endl;
		if (piece->canMove()) return false;
	}
	return true;
}

bool Board::stalemate(bool colour) const {
	// Invariant: this method should only ever be called if colour isn't in check, so we won't check for that
	for (auto piece: !colour ? whitePieces : blackPieces) {
		if (piece->canMove()) return false;
	}
	bool bishopFound = false, knightFound = false;
	for (auto piece: whitePieces) {
		switch (piece->getName()) {
			case 'K':
				continue;
			case 'N':
				if (knightFound || bishopFound) return false;
				else knightFound = true;
			case 'B':
				if (bishopFound) continue;
				else if (knightFound) return false;
				else bishopFound = true;
			default:
				return false;
		}
	}
	for (auto piece: blackPieces) {
		switch (piece->getName()) {
			case 'K':
				continue;
			case 'N':
				if (knightFound || bishopFound) return false;
				else knightFound = true;
			case 'B':
				if (bishopFound) continue;
				else if (knightFound) return false;
				else bishopFound = true;
			default:
				return false;
		}
	}
	bishopFound = knightFound = false;
	return true;
}

bool Board::repetition() const {
	if (log.size() < 9) return false;
	// Fill in
}

Board::Iterator::Iterator(const Board &board, const std::vector<Move> &log, bool begin):
  i{begin ? 0 : log.size() - 1}, board(const_cast<Board&>(board)), log{log} {}

const Board& Board::Iterator::operator*() const {
	return board;
}

Board::Iterator& Board::Iterator::operator++() {
	if (i == log.size() - 1) throw BadMove{emptyMove};
	board.movePiece({log[i].oldPos, log[i].newPos, log[i].promotion, log[i].capture});
	i++;
	board.turn = !board.turn;
	return *this;
}

Board::Iterator& Board::Iterator::operator--() {
	if (!i) throw BadMove{emptyMove};
	board.undoMoves();
	i--;
	board.turn = !board.turn;
	return *this;
}

bool Board::Iterator::operator!=(const Iterator &other) const {
	return i != other.i && log != other.log;
}

Board::Iterator Board::begin(const Board &board) const {
	return {*this, board.log, true};
}

Board::Iterator Board::end(const Board &board) const {
	return {*this, board.log, false};
}

int Board::runCalculations() {
    std::vector<Posn> defensivePositions;
	std::shared_ptr<King> tmpWhite = std::static_pointer_cast<King>(whiteKing);
	std::shared_ptr<King> tmpBlack = std::static_pointer_cast<King>(blackKing);
	// std::cerr << 1 << std::endl;
	for (auto p: whitePieces) p->protect(false);
	for (auto p: blackPieces) p->protect(false);
	// std::cerr << 2 << std::endl;
	// std::cerr << whiteKing << std::endl << blackKing << std::endl;
	bool inCheck = (turn ? tmpWhite : tmpBlack)->calculatePins(*this, defensivePositions);
	// std::cerr << 2.5 << std::endl;
	(!turn ? tmpWhite : tmpBlack)->calculatePins(*this, defensivePositions);
	// std::cerr << 3 << std::endl;
	for (auto p: whitePieces) {
		// std::cerr << p->getName() << " at " << char('a' + p->getX()) << p->getY() + 1 << ": ";
		if (p->getName() != 'K') {
			p->calculateLegalMoves(*this);
			// std::cerr << "success";
			if (turn && inCheck) {
				std::cerr << " but we're checking here?";
				p->intersect(defensivePositions);
			}
			// std::cerr << std::endl;
		}
	}
	// std::cerr << 4 << std::endl;
	for (auto p: blackPieces) {
		if (p->getName() != 'k') {
			// std::cerr << p->getName() << std::endl;
			p->calculateLegalMoves(*this);
			if (!turn && inCheck) {
				// std::cerr << "what" << std::endl;
				p->intersect(defensivePositions);
			}
		}
	}
	// std::cerr << 5 << std::endl;
	whiteKing->calculateLegalMoves(*this);
	// std::cerr << "White king's moves: ";
	// for (auto p: whiteKing->getLegalMoves()) std::cerr << char('a' + p.x) << p.y + 1 << ", ";
	blackKing->calculateLegalMoves(*this);
	// std::cerr << std::endl << "Black king's moves: ";
	// for (auto p: blackKing->getLegalMoves()) std::cerr << char('a' + p.x) << p.y + 1 << ", ";
	// std::cerr << std::endl;
	if (inCheck) {
		if (checkmate(turn)) {
			// std::cerr << "checkmate" << std::endl;
			return 2; // checkmate
		} else {
			// std::cerr << "check" << std::endl;
			return 1; // check
		}
	} else if (stalemate(turn)) {
		// std::cerr << "stalemate" << std::endl;
		return 0; // stalemate
	} else {
		// std::cerr << "nothign" << std::endl;
		return -1; // nothing
	}
}

void Board::movePiece(Move &&move) {
	std::cerr << "start" << std::endl;
	if (!board[move.oldPos.x][move.oldPos.y]) {
		std::cerr << "FIRST" << std::endl;
		throw BadMove{move};
  } else if (board[move.oldPos.x][move.oldPos.y]->getColour() != turn) {
		std::cerr << "SECOND" << std::endl;
		throw BadMove{move};
  } else if (!board[move.oldPos.x][move.oldPos.y]->canMoveTo(move.newPos)) {
		// std::cerr << "Legal moves of " << board[move.oldPos.x][move.oldPos.y]->getName() << ": ";
		// for (auto i : board[move.oldPos.x][move.oldPos.y]->getLegalMoves()) {
		// 	std::cerr << char('a' + i.x) << i.y + 1 << " ";
		// }
		std::cerr << "THIRD" << std::endl;
		throw BadMove{move};
	}
	std::cerr << "good" << std::endl;
	if (board[move.newPos.x][move.newPos.y]) { // if a capture is taking place
		std::cerr << "capture" << std::endl;
		deadPieces.emplace_back(board[move.newPos.x][move.newPos.y]);
		removePiece(move.newPos);
		move.capture = true;
	} else if (board[move.oldPos.x][move.oldPos.y]->getName() == (turn ? 'P' : 'p') && abs(move.newPos.x - move.oldPos.x) == 1) {
		deadPieces.emplace_back(board[move.newPos.x][move.newPos.y + (turn ? -1 : 1)]);
		removePiece({move.newPos.x, move.newPos.y + (turn ? -1 : 1)});
		move.capture = true;
	}
	std::cerr << "up" << std::endl;
	(board[move.newPos.x][move.newPos.y] = board[move.oldPos.x][move.oldPos.y])->move(move.newPos); // move the piece
	removePiece(move.oldPos);
	std::cerr << "down" << std::endl << "position is " << char('a' + move.newPos.x) << move.newPos.y + 1 << std::endl;
	std::cerr << "Piece at new position is " << board[move.newPos.x][move.newPos.y]->getName() << std::endl << "nice" << std::endl;
	bool castling = false;
	if (board[move.newPos.x][move.newPos.y]->getName() == (turn ? 'K' : 'k')) { // check for castling
		std::cerr << "we found a king" << std::endl;
		if (move.newPos.x - move.oldPos.x == 2) { // castling right
			std::cerr << "we're castling??" << std::endl;
			std::cerr << char('a' + WIDTH - 1) << move.newPos.y + 1 << " --> " << char('a' + move.newPos.x - 1) << move.newPos.y + 1 << std::endl;
			movePiece({{WIDTH - 1, move.newPos.y}, {move.newPos.x - 1, move.newPos.y}}); // move the rook
			castling = true;
			log.pop_back();
		} else if (move.oldPos.x - move.newPos.x == 2) { // castling left
			std::cerr << "we're castling??" << std::endl;
			std::cerr << 'a' << move.newPos.y + 1 << " --> " << char('a' + move.newPos.x + 1) << move.newPos.y + 1 << std::endl;
			movePiece({{0, move.newPos.y}, {move.newPos.x + 1, move.newPos.y}}); // move the rook
			castling = true;
			log.pop_back();
		}
	}
	log.emplace_back(move); // log move
	if (move.promotion) promote(move.newPos, move.promotion);
	if (!castling) turn = !turn;
	std::cerr << "end" << std::endl;
}

void Board::removePiece(const Posn &posn) {
	if (!board[posn.x][posn.y]) return;
	// std::cerr << "hmm" << std::endl;
	bool colour = board[posn.x][posn.y]->getColour();
	if (board[posn.x][posn.y]->getName() == (colour ? 'K' : 'k')) {
		(colour ? whiteKing : blackKing) = emptyptr;
	}
	// std::cerr << "hhmmm" << std::endl;
	for (auto it = (colour ? whitePieces : blackPieces).begin(); it != (colour ? whitePieces : blackPieces).end(); it++) {
		if ((*it)->getPosn() == posn) {
			// std::cerr << "hhhmmmm" << std::endl;
			std::cerr << "Have found a " << (*it)->getName() << " in " << (colour ? "whitepieces" : "blackpieces") << std::endl;
			(colour ? whitePieces : blackPieces).erase(it);
			std::cerr << "Next thing is now " << (*it)->getName() << std::endl;
			break;
		} else if (/*(std::cerr << "bruh how" << std::endl) && */(*it)->getName() == (colour ? 'K' : 'k')) {
			(colour ? whiteKing : blackKing) = *it;
		}
	}
	// std::cerr << "???" << std::endl;
	board[posn.x][posn.y] = emptyptr;
	// std::cerr << "ur kidding" << std::endl;
}

const std::shared_ptr<Piece> Board::operator[](const Posn &posn) const {
	return board[posn.x][posn.y];
}

Move Board::getLastMove() const {
	return log.empty() ? emptyMove : log.back();
}

void Board::undoMoves(int num) {
	for (int i = 0; i < num; i++) {
		if (log.empty()) break;
    	turn = !turn;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];
		board[log.back().oldPos.x][log.back().oldPos.y]->move(log.back().oldPos, false);
		removePiece(log.back().newPos);
		if (log.back().capture) {
			addPieceHelp(deadPieces.back()->getName(), deadPieces.back()->getPosn());
			insert(deadPieces.back()->getColour() ? whitePieces : blackPieces, deadPieces.back());
			deadPieces.pop_back();
		}
		if (log.back().promotion) {
			removePiece(log.back().oldPos);
			addPiece<Pawn>(turn, log.back().oldPos);
		}
		if (board[log.back().oldPos.x][log.back().oldPos.y]->getName() == (turn ? 'K' : 'k')) { // check for castling
			std::cerr << "we found a not king" << std::endl;
			if (log.back().newPos.x - log.back().oldPos.x == 2) { // castling right
				std::cerr << "we're uncastling??" << std::endl;
				board[WIDTH - 1][log.back().oldPos.y] = board[log.back().newPos.x - 1][log.back().newPos.y];
				board[WIDTH - 1][log.back().oldPos.y]->move({WIDTH - 1, log.back().oldPos.y}, false); // move the rook
				log.pop_back();
			} else if (log.back().oldPos.x - log.back().newPos.x == 2) { // castling left
				std::cerr << "we're uncastling??" << std::endl;
				board[0][log.back().oldPos.y] = board[log.back().newPos.x + 1][log.back().newPos.y];
				board[0][log.back().oldPos.y]->move({0, log.back().oldPos.y}, false); // move the rook
				log.pop_back();
			}
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

void Board::setShowDead(bool show) {
	showDead = show;
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
		if (board.showDead && row == 1) {
			out << "   ";
			for (auto p: board.deadPieces) {
				if (p->getColour()) {
					out << p->getName(); // printing out the pieces black has captured
				}
			}
		} else if (board.showDead && row == HEIGHT - 2) {
			out << "   ";
			for (auto p: board.deadPieces) {
				if (!p->getColour()) {
					out << p->getName(); // printing out the pieces white has captured
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
