// include/piece.h
#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <iostream>

class Piece {
protected:
	const unsigned int MAX_X = 8;
	const unsigned int MAX_Y = 8;

	bool colour;
	std::string name;

	unsigned int posX;
	unsigned int posY;
public:
	Piece(bool colour, std::string name, unsigned int posX, unsigned int posY);

	// Getter
	unsigned int getX(void) const;
	unsigned int getY(void) const;
	std::string getName(void) const;
	bool getColour(void) const;

	// Setter
	void setX(unsigned int value);
	void setY(unsigned int value);
	void setName(std::string& value);
};

std::ostream& operator<<(std::ostream& out, const Piece& p);

#endif // PIECE_H
