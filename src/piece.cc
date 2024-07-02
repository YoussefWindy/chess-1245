// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, int posX, int posY) : name{name}, posX{static_cast<unsigned int>(posX)}, posY{static_cast<unsigned int>(posY)} {
	if ('a' <= name && name <= 'z') {
		colour = 0;
	} else {
		colour = 1;
	}
}

// Getter
char Piece::getName(void) const {
	return this->name;
}

bool Piece::getColour(void) const {
	return this->colour;
}

unsigned int Piece::getX(void) const {
	return this->posX;
}

unsigned int Piece::getY(void) const {
	return this->posY;
}

// Setter
void Piece::setName(char value) {
	this->name = value;
}

void Piece::setX(unsigned int posX) {
	if (posX < this->MAX_X) {
		this->posX = posX;
	}
}

void Piece::setY(unsigned int posY) {
	if (posY < this->MAX_Y) {
		this->posY = posY;
	}
}

std::ostream& operator<<(std::ostream& out, const Piece& p) {
	out << p.getName();

	return out;
}
