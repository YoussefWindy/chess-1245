// include/piece.h
#ifndef PIECE_H
#define PIECE_H

#include "utilities.h"

class Board; // forward declaration

class Piece {
  protected:
	char name;
	bool colour;
	Posn posn;
	std::vector<Posn> legalMoves;
	bool hasPath = false;
  public:
	Piece(char name, bool colour, const Posn &posn);
	virtual void calculateLegalMoves(const Board &board) = 0;
	virtual int calculateNumPinned(const Board &board) const = 0;
	bool canMoveTo(const Posn &posn) const;
	bool canMove() const;

	// Getters
	char getName() const;
	bool getColour() const;
	Posn getPosn() const;
	unsigned int getX() const;
	unsigned int getY() const;

	// Setters
	void setName(char value);
	void setPosn(const Posn &posn);
	void setX(unsigned int value);
	void setY(unsigned int value);
};

std::ostream& operator<<(std::ostream &out, const Piece &posn);

const std::shared_ptr<Piece> emptyptr = std::shared_ptr<Piece>{}; // empty std::shared_ptr<Piece>, kind of similar to nullptr

#endif // PIECE_H
