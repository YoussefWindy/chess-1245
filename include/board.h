// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"

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

	// Piece methods
	void addPiece(char name, const Posn &posn);
	bool movePiece(const Move &move); // returns true if move is valid, false otherwise
	void removePiece(const Posn &posn);

	// Getter/Checker methods
	const std::shared_ptr<Piece> (&getBoard() const)[HEIGHT][WIDTH]; //  we'll see if we need this or not, the below fn kind of make it obsolete
	const std::shared_ptr<Piece> operator[](const Posn &posn) const;
	bool positionInCheck(const Posn &posn, bool colour) const;
	bool checkmate(bool colour) const; // needs work
	// discuss above two methods
	bool validate() const; // needs work
};

std::ostream& operator<<(std::ostream& out, const Board& board);

#endif // BOARD_H
