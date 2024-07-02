// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, int posX, int posY) : name{name}, posX{posX}, posY{posY} {
	if ('b' <= name && name <= 'r') {
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
	this->posX = posX;
}

void Piece::setY(unsigned int posY) {
	this->posY = posY;
}

std::ostream& operator<<(std::ostream& out, const Piece& p) {
	out << p.getName();

	return out;
}
