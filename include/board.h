// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Board {
	std::shared_ptr<Piece> board[WIDTH][HEIGHT];
	std::vector<std::shared_ptr<Piece>> whitePieces, blackPieces, deadPieces;
	std::vector<Move> log;
  public:
	Board();
	~Board();

	class Iterator {
		friend class Board;
		int i, j;
		const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT];
		Iterator(const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin);
	  public:
		std::shared_ptr<Piece> operator*() const;
		Iterator& operator++();
		bool operator!=(const Iterator &other) const;
	};

	Iterator begin() const;
	Iterator end() const;

	// Move methods
	bool validateMove(Move) const; // Not implemented yet
	void doMove(Move); // Not implemented yet

	// Setup methods
	void addPiece(const char name, const Posn pos);
	void removePiece(const Posn pos);

	// Getter/Checker methods
	const std::shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH];
	bool positionInCheck(Posn pos, bool colour) const;
	bool checkmate(bool colour) const; // needs work
	// discuss above two methods
	bool validate() const; // needs work
};

std::ostream& operator<<(std::ostream& out, const Board& b);

#endif // BOARD_H
