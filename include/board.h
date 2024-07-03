// include/board.h
#ifndef BOARD_H
#define BOARD_H

#include "../include/piece.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Board {
  private:
	std::shared_ptr<Piece> board[WIDTH][HEIGHT];
  public:
	Board();
	~Board();
	struct Posn {
		int x, y;
		bool validate() const;
	};

	class Iterator {
		friend class Board;
		int i, j;
		const std::shared_ptr<Piece> (&board)[WIDTH][HEIGHT];
		Iterator(const shared_ptr<Piece> (&board)[WIDTH][HEIGHT], bool begin);
	  public:
		std::shared_ptr<Piece> operator*() const;
		Iterator& operator++();
		bool operator!=(const Iterator &) const;
	};

	Iterator begin() const;
	Iterator end() const;

	// Methods
	void addPiece(const char name, const Posn pos);
	void kill(const Posn pos);

	bool validate() const;
	const std::shared_ptr<Piece> (&Board::getBoard() const)[HEIGHT][WIDTH];
};

std::ostream& operator<<(std::ostream& out, const Board& b);

#endif // BOARD_H
