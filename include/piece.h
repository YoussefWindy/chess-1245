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
	bool vertical, horizontal, positive, negative;
	bool hasMoved, isProtected;
  public:
	Piece(char name, bool colour, const Posn &posn, bool vertical = false,
  	  bool horizontal = false, bool positive = false, bool negative = false);
	virtual void calculateLegalMoves(const Board &board) = 0;
	bool canMoveTo(const Posn &posn) const;
	bool canMove() const;

	// Getters
	char getName() const;
	bool getColour() const;
	Posn getPosn() const;
	unsigned int getX() const;
	unsigned int getY() const;
	bool getHasMoved() const;
	bool getIsProtected() const;

	// Setters
	void move(const Posn &posn);
	void setName(char value);
	void setX(unsigned int value);
	void setY(unsigned int value);
	void protect(bool isProtected = true);
};

std::ostream& operator<<(std::ostream &out, const Piece &posn);

const std::shared_ptr<Piece> emptyptr = std::shared_ptr<Piece>{}; // empty std::shared_ptr<Piece>, kind of similar to nullptr

#endif // PIECE_H
