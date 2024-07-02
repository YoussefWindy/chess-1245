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

	unsigned int posX;
	unsigned int posY;
public:
	Piece(char name, int posX, int posY);

	// Getter
	char getName(void) const;
	bool getColour(void) const;
	unsigned int getX(void) const;
	unsigned int getY(void) const;

	// Setter
	void setName(char value);
	void setX(unsigned int value);
	void setY(unsigned int value);
};

std::ostream& operator<<(std::ostream& out, const Piece& p);

#endif // PIECE_H
