// include/piece.h
#ifndef PIECE_H
#define PIECE_H

#include <iostream>

class Piece {
  protected:
	const unsigned int MAX_X = 8;
	const unsigned int MAX_Y = 8;

	bool colour;
	char name;
	Board::Posn posn;
	std::vector<Board::Posn> legalMoves;
  public:
	Piece(char name, Board::Posn posn);

	// Getters
	char getName() const;
	bool getColour() const;
	Board::Posn getPosn() const;
	unsigned int getX() const;
	unsigned int getY() const;

	// Setters
	void setName(char value);
	void setPosn(Board::Posn posn);
	void setX(unsigned int value);
	void setY(unsigned int value);
};

std::ostream& operator<<(std::ostream& out, const Piece& p);

#endif // PIECE_H
