// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, bool colour, const Posn &posn, bool v, bool h, bool p, bool n): name{name}, colour{colour},
  posn{posn}, vertical{v}, horizontal{h}, positive{p}, negative{n}, hasMoved{false}, isProtected{false} {}

bool Piece::canMoveTo(const Posn &posn) const {
	for (Posn posn: legalMoves) {
		if (this->posn == posn) return true;
	}
	return false;
}

bool Piece::canMove() const {
	return !legalMoves.empty();
}

// Getters
char Piece::getName() const {
	return name;
}

bool Piece::getColour() const {
	return colour;
}

Posn Piece::getPosn() const {
	return posn;
}

unsigned int Piece::getX() const {
	return posn.x;
}

unsigned int Piece::getY() const {
	return posn.y;
}

bool Piece::getHasMoved() const {
	return hasMoved;
}

bool Piece::getIsProtected() const {
	return isProtected;
}

// Setters
void Piece::move(const Posn &posn) {
	hasMoved = true;
	this->posn = posn;
}

void Piece::setName(char value) {
	this->name = value;
}

void Piece::setX(unsigned int posX) {
	this->posn.x = posX;
}

void Piece::setY(unsigned int posY) {
	this->posn.y = posY;
}

void Piece::protect(bool isProtected) {
	this->isProtected = isProtected;
}

std::ostream& operator<<(std::ostream &out, const Piece &posn) {
	out << posn.getName();
	return out;
}
