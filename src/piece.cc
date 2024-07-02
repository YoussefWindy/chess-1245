// src/piece.cc

#include "../include/piece.h"

#include <string>

Piece::Piece(bool colour, std::string name, unsigned int posX, unsigned int posY) : colour{colour}, name{name}, posX{posX}, posY{posY} {}

// Getter
unsigned int Piece::getX(void) const {
	return this->posX;
}

unsigned int Piece::getY(void) const {
	return this->posY;
}

std::string Piece::getName(void) const {
	return this->name;
}

bool Piece::getColour(void) const {
	return this->colour;
}

// Setter
void Piece::setX(unsigned int value) {
	if (value < MAX_X) {
		this->posX = value;
	}
}

void Piece::setY(unsigned int value) {
	if (value < MAX_Y) {
		this->posY = value;
	}
}

void Piece::setName(std::string& value) {
	this->name = value;
}

std::ostream& operator<<(std::ostream& out, const Piece& p) {
	out << "Name: " << p.getName() << std::endl;
	out << "Colour: ";
	out << (p.getColour() == 0 ? "Black" : "White");
	out << std::endl;
	out << "Position: (" << p.getX();
	out << ", " << p.getY() << ')' << std::endl;

	return out;
}
