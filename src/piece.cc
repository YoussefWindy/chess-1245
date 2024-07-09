// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, Posn): name{name}, posn{posn} {
	if ('b' <= name && name <= 'r') {
		colour = 0;
	} else {
		colour = 1;
	}
}

bool Piece::canReach(Posn pos) const {
	for (Posn posn: legalMoves) {
		if (pos == posn) return true;
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

// Setters
void Piece::setName(char value) {
	this->name = value;
}

void Piece::setPosn(Posn posn) {
	this->posn = posn;
}

void Piece::setX(unsigned int posX) {
	this->posn.x = posX;
}

void Piece::setY(unsigned int posY) {
	this->posn.y = posY;
}

std::ostream& operator<<(std::ostream& out, const Piece& p) {
	out << p.getName();

	return out;
}
