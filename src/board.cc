// src/board.cc

#include "../include/piece.h"
#include "../include/board.h"
#include "../include/board.tpp"
#include <iostream>
#include <ostream>
#include <memory>

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
		deadPieces.emplace_back(board[p->getX()][p->getY()]);
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
		if (piece->getValue() == 1) {
			if (abs(posn.x - piece->getX()) == 1 && (!colour ? posn.y - piece->getY() : piece->getY() - posn.y) == 1) {
				return true;
			}
		} else if (piece->canMoveTo(posn)) {
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
	bool noMoves = true, noMaterial = true;
	for (auto piece: !colour ? whitePieces : blackPieces) {
		if (piece->canMove()) {
			noMoves = false;
			break;
		}
	}
	std::shared_ptr<Piece> bishopFound = emptyptr;
	bool knightFound = false;
	for (auto piece: whitePieces) {
		if (!noMaterial) break;
		switch (piece->getName()) {
			case 'K':
				// std::cerr << "gooda" << std::endl;
				continue;
			case 'N':
				// std::cerr << "bad1a" << std::endl;
				if (knightFound || bishopFound) noMaterial = false;
				else knightFound = true;
				break;
			case 'B':
				// std::cerr << "bad2a" << std::endl;
				if (knightFound || (bishopFound && (bishopFound->getColour() == piece->getColour() ||
				  (bishopFound->getX() + bishopFound->getY()) % 2 != (piece->getX() + piece->getY()) % 2))) {
					noMaterial = false;
				}
				else bishopFound = piece;
				break;
			default:
				// std::cerr << "bad3a" << std::endl;
				noMaterial = false;
		}
	}
	for (auto piece: blackPieces) {
		if (!noMaterial) break;
		switch (piece->getName()) {
			case 'k':
				// std::cerr << "goodb" << std::endl;
				continue;
			case 'n':
				// std::cerr << "bad1b" << std::endl;
				if (knightFound || bishopFound) noMaterial = false;
				else knightFound = true;
				break;
			case 'b':
				// std::cerr << "bad2b" << std::endl;
				if (knightFound || (bishopFound && (bishopFound->getColour() == piece->getColour() ||
				  (bishopFound->getX() + bishopFound->getY()) % 2 != (piece->getX() + piece->getY()) % 2))) {
					noMaterial = false;
				}
				else bishopFound = piece;
				break;
			default:
				// std::cerr << "bad3b" << std::endl;
				noMaterial = false;
		}
	}
	return noMoves || noMaterial;
}

bool Board::repetition() const {
	if (log.size() < 9) return false;
	
  auto move_1 = std::make_unique<Move>(log[log.size() - 9]);
  auto move_2 = std::make_unique<Move>(log[log.size() - 8]);
  auto move_3 = std::make_unique<Move>(log[log.size() - 7]);
  auto move_4 = std::make_unique<Move>(log[log.size() - 6]);
  auto move_5 = std::make_unique<Move>(log[log.size() - 5]);
  auto move_6 = std::make_unique<Move>(log[log.size() - 4]);
  auto move_7 = std::make_unique<Move>(log[log.size() - 3]);
  auto move_8 = std::make_unique<Move>(log[log.size() - 2]);
  auto move_9 = std::make_unique<Move>(log[log.size() - 1]);

  if (move_9->newPos == move_7->oldPos &&
      move_9->newPos == move_5->newPos &&
      move_9->newPos == move_3->oldPos &&
      move_9->oldPos == move_7->newPos &&
      move_9->oldPos == move_5->oldPos &&
      move_9->oldPos == move_3->newPos &&
      move_8->newPos == move_6->oldPos &&
      move_8->newPos == move_4->newPos &&
      move_8->newPos == move_2->oldPos &&
      move_8->oldPos == move_6->newPos &&
      move_8->oldPos == move_4->oldPos &&
      move_8->oldPos == move_2->newPos) return true;

	return false;
}

void Board::promote(const Posn &posn, unsigned int type) {
	std::cerr << "type is " << type << ". turn is " << (turn ? "white" : "black") << std::endl;
	removePiece(posn);
	std::cerr << "this work" << std::endl;
	switch (type) {
		case 1:
			std::cerr << 1 << std::endl;
			addPiece<Knight>(turn, posn);
			break;
		case 2:
			std::cerr << 2 << std::endl;
			addPiece<Bishop>(turn, posn);
			break;
		case 3:
			std::cerr << 3 << std::endl;
			addPiece<Rook>(turn, posn);
			break;
		case 4:
			std::cerr << 4 << std::endl;
			addPiece<Queen>(turn, posn);
			break;
	}
	std::cerr << char('a' + log.back().oldPos.x) << log.back().oldPos.y + 1
	  << "-->" << char('a' + log.back().newPos.x) << log.back().newPos.y + 1 << " "
	  << "capture: " << log.back().capture << ", promotion: " << log.back().promotion << std::endl;
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
				std::cerr << " but we're checking here?" << std::endl;
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
				std::cerr << " but we're checking here?" << std::endl;
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
	if (repetition()) return 0;
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
	std::cerr << "All pieces in " << (colour ? "whitepieces" : "blackpieces") << " BEFORE: ";
	for (auto p: colour ? whitePieces : blackPieces) {
		std::cerr << p->getName() << " (" << char(p->getX() + 'a') << p->getY() + 1 << ") ";
	}
	std::cerr << std::endl;
	for (auto it = (colour ? whitePieces : blackPieces).begin(); it != (colour ? whitePieces : blackPieces).end(); it++) {
		if ((*it)->getPosn() == posn) {
			// std::cerr << "hhhmmmm" << std::endl;
			// std::cerr << "Have found a " << (*it)->getName() << " in " << (colour ? "whitepieces" : "blackpieces") << std::endl;
			// // (colour ? whitePieces : blackPieces).erase(it);
			// std::cerr << "Next thing is now " << (*it)->getName() << std::endl;
			if (colour) {
			std::cerr << "Have found a " << (*it)->getName() << " (" << char((*it)->getX() + 'a') << (*it)->getY() + 1 << ") " << "in whitepieces" << std::endl;
				whitePieces.erase(it);
			std::cerr << "Next thing is now " << (*it)->getName() << " (" << char((*it)->getX() + 'a') << (*it)->getY() + 1 << ") " << std::endl;
			} else {
			std::cerr << "Have found a " << (*it)->getName() << " (" << char((*it)->getX() + 'a') << (*it)->getY() + 1 << ") " << "in blackpieces" << std::endl;
				blackPieces.erase(it);
			std::cerr << "Next thing is now " << (*it)->getName() << " (" << char((*it)->getX() + 'a') << (*it)->getY() + 1 << ") " << std::endl;
			}
			break;
		} else if (/*(std::cerr << "bruh how" << std::endl) && */(*it)->getName() == (colour ? 'K' : 'k')) {
			(colour ? whiteKing : blackKing) = *it;
		}
	}
	std::cerr << "All pieces in " << (colour ? "whitepieces" : "blackpieces") << " AFTER: ";
	for (auto p: colour ? whitePieces : blackPieces) {
		std::cerr << p->getName() << " (" << char(p->getX() + 'a') << p->getY() + 1 << ") ";
	}
	std::cerr << std::endl;
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
  	// Undo given number of moves
	for (int i = 0; i < num; i++) {
    	// If the move log is empty, break and exit
		if (log.empty()) break;

    	// Update the current turn
    	turn = !turn;
    
    	// Check for promotion
		if (log.back().promotion) {
			std::cerr << "undoing a promotion of a " << board[log.back().newPos.x][log.back().newPos.y]->getName() << std::endl;
			removePiece(log.back().newPos);
			addPiece<Pawn>(turn, log.back().newPos);
			std::cerr << "it is now a " << board[log.back().newPos.x][log.back().newPos.y]->getName() << std::endl;
		}
    
    	// Copy the piece back into the old position
		std::cerr << "internal position: " << char('a' + board[log.back().newPos.x][log.back().newPos.y]->getX())
				  << board[log.back().newPos.x][log.back().newPos.y]->getY() + 1 << std::endl;
		board[log.back().oldPos.x][log.back().oldPos.y] = board[log.back().newPos.x][log.back().newPos.y];

    	// update piece's internal position and move count
		board[log.back().oldPos.x][log.back().oldPos.y]->move(log.back().oldPos, false);
		removePiece(log.back().newPos);
		std::cerr << "internal position: " << char('a' + board[log.back().oldPos.x][log.back().oldPos.y]->getX())
				  << board[log.back().oldPos.x][log.back().oldPos.y]->getY() + 1 << std::endl;

    	// Retrieve capturd piece from the graveyard
		if (log.back().capture) {
			addPieceHelp(deadPieces.back()->getName(), deadPieces.back()->getPosn());
			deadPieces.pop_back();
		}
    
    	// Check for castling
		if (board[log.back().oldPos.x][log.back().oldPos.y]->getName() == (turn ? 'K' : 'k')) {
      		// Debug statement
			std::cerr << "we found a not king" << std::endl;
      
      		// Castling right
			if (log.back().newPos.x - log.back().oldPos.x == 2) {
        		// Debug statement
				std::cerr << "we're uncastling??" << std::endl;
        
        		// Move the pieces into old positions
				board[WIDTH - 1][log.back().oldPos.y] = board[log.back().newPos.x - 1][log.back().newPos.y];
				board[WIDTH - 1][log.back().oldPos.y]->move({WIDTH - 1, log.back().oldPos.y}, false); // move the rook
			} else if (log.back().oldPos.x - log.back().newPos.x == 2) { // castling left
        		// Debug statement
				std::cerr << "we're uncastling??" << std::endl;
        
        		// Move the pieces into old positions
				board[0][log.back().oldPos.y] = board[log.back().newPos.x + 1][log.back().newPos.y];
				board[0][log.back().oldPos.y]->move({0, log.back().oldPos.y}, false); // move the rook
			}
		}
    
    	// Remove from log
		log.pop_back();
	}
  
  // Recalculate legal moves
  runCalculations();
}

bool Board::getTurn() const {
  return turn;
}

void Board::validate() {
  // Check if both Kings exist on the board
	if (!hasKing(true)) throw BadSetup{"no white king on the board!"};
	if (!hasKing(false)) throw BadSetup{"no black king on the board!"};

  // Check white pawns
	for (auto p: whitePieces) { 
		if (p->getName() == 'P') {
			Posn pawnPosn = p->getPosn();
      // White pawn in promotion zone
			if (pawnPosn.y == HEIGHT - 1) throw BadSetup{"there is a white pawn on the last row!"};
		}
	}

  // Check black pawns
	for (auto p: blackPieces) { 
		if (p->getName() == 'p') {
			Posn pawnPosn = p->getPosn();
      // Black pawn in promotion zone
			if (!pawnPosn.y) throw BadSetup{"there is a black pawn on the last row!"};
		}
	}

  // Ensure no Kings are in check on setup
	if (runCalculations() > 0) throw BadSetup{std::string((!turn ? "white" : "black")) + " is in check!"};

	// Swap turn to check other king
  turn = !turn;

	if (runCalculations() > 0) {
    // Revert turn to correct
		turn = !turn;
		throw BadSetup{std::string((!turn ? "white" : "black")) + " is in check!"};
	}

  // Revert turn to correct turn
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

  // Iterate over the rows
	for (unsigned int row = 0; row < HEIGHT; row++) { 
    // Row number
		out << (HEIGHT - row) << " ";
		
    // Iterate over the columns
    for (unsigned int col = 0; col < WIDTH; col++) {
			if (board[{col, HEIGHT - row - 1}]) {
        // if there's a piece there, print it
				out << board[{col, HEIGHT - row - 1}]->getName();
			} else {
        // if it's a blank space, print it
        // print black or white square
				out << ((col + row) % 2 ? '_' : ' ');
			}
		}
    
    // Print dead pieces
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
	
  // Print column letters
  for (unsigned char c = 'a'; c < 'a' + WIDTH; c++) {
		out << c;
	}
	out << std::endl;
	return out;
}
