// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"

class Board {
	friend class King;
	friend class AI;
	std::shared_ptr<Piece> board[WIDTH][HEIGHT];
	std::vector<std::shared_ptr<Piece>> whitePieces, blackPieces, deadPieces;
	std::shared_ptr<King> whiteKing, blackKing;
	std::vector<Move> log;

	// Checking for check and checkmate
	bool check(const Posn &posn, bool colour) const;
	bool checkmate(bool colour) const; // needs work
	bool stalemate(bool colour) const;
  public:
	Board();
	Board(const Board &);
	Board(Board &&); //  I don't think this is ever used, but Rule of Big 5 says we have to implement it
	void swap(Board &, Board &);
	Board& operator=(const Board &);
	Board& operator=(Board &&);
	~Board();

	class Iterator {
		friend class Board;
		unsigned int i, j;
		const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT];
		Iterator(const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin);
	  public:
		std::shared_ptr<Piece> operator*() const;
		Iterator& operator++();
		bool operator!=(const Iterator &other) const;
	};

	Iterator begin() const;
	Iterator end() const;

	// THE BIG BOY METHOD - handles all end of turn calculations
	int runCalculations(bool colour);

	// Piece methods
	template <typename T>
	void addPiece(bool colour, const Posn &posn);
	void movePiece(Move &&move); // will throw a BadMove exception if move is invalid
	void removePiece(const Posn &posn);
	template <typename T>
	void promote(bool colour, const Posn &posn);
	bool undoMoves(int num); // returns true is num is less than the number of moves played so far, false otherwise

	// Getter methods
	const std::shared_ptr<Piece> operator[](const Posn &posn) const;
	Move getLastMove() const;

	// Other checking methods
	bool validate() const; // needs work
	bool hasKing(bool colour) const;
};

std::ostream& operator<<(std::ostream& out, const Board& board);

#include "board.tpp"

#endif // BOARD_H
