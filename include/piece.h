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
	std::vector<Posn> legalMoves, path;
	bool hasMoved, isProtected, hasPathToKing;
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
	bool getHasMoved() const;
	bool getIsProtected() const;
	bool getHasPathToKing() const;

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
