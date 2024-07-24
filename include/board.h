// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"

class Board {
	friend class AI;
	friend class King;
	friend class XWindow;
	friend std::ostream& operator<<(std::ostream& out, const Board& board);
	std::shared_ptr<Piece> board[WIDTH][HEIGHT];
	std::vector<std::shared_ptr<Piece>> whitePieces, blackPieces, deadPieces;
	std::shared_ptr<Piece> whiteKing, blackKing;
  bool turn;
	std::vector<Move> log;

	void addPieceHelp(char name, const Posn &posn);
	void movePiece(const Move &);
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
	~Board() = default;

	class Iterator { // Iterator class for replay functionality
		friend class Board;
		long unsigned int i;
		Board &board;
		const std::vector<Move> &log;
		Iterator(const Board &board, const std::vector<Move> &log, bool begin);
	  public:
		const Board& operator*() const;
		Iterator& operator++();
		Iterator& operator--();
		bool operator!=(const Iterator &other) const;
	};

	Iterator begin(const Board &board) const;
	Iterator end(const Board &board) const;

	// THE BIG BOY METHOD - handles all end of turn calculations
	int runCalculations();

	// Piece methods
	template <typename T>
	void addPiece(bool colour, const Posn &posn);
	void movePiece(Move &&move); // will throw a BadMove exception if move is invalid
	void removePiece(const Posn &posn);
	void promote(const Move &move, unsigned int type);
	void undoMoves(int num = 1); // returns true if num is less than the number of moves played so far, false otherwise

	// Getter methods
	const std::shared_ptr<Piece> operator[](const Posn &posn) const;
	Move getLastMove() const;
  	bool getTurn() const;

	// Other checking methods
	void validate(); // Will throw a BadSetup exception if setup is invalid
	bool hasKing(bool colour) const;

	void setTurn(bool colour);

	const std::vector<Move> getLog() const; // I don't like this
};

std::ostream& operator<<(std::ostream& out, const Board& board);

#endif // BOARD_H
