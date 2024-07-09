// include/piece.h
#ifndef PIECE_H
#define PIECE_H

#include "utilities.h"

#include <iostream>
#include <vector>

class Piece {
  protected:
	bool colour;
	char name;
	Posn posn;
	std::vector<Posn> legalMoves;
  public:
	Piece(char name, Posn posn);
	virtual void calculateLegalMoves() = 0;
	bool canReach(Posn pos) const;
	bool canMove() const;

	// Getters
	char getName() const;
	bool getColour() const;
	Posn getPosn() const;
	unsigned int getX() const;
	unsigned int getY() const;

	// Setters
	void setName(char value);
	void setPosn(Posn posn);
	void setX(unsigned int value);
	void setY(unsigned int value);
};

std::ostream& operator<<(std::ostream& out, const Piece& p);

#endif // PIECE_H
