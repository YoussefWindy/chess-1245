// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, bool colour, const Posn &posn): name{name}, colour{colour}, posn{posn} {}

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

// Setters
void Piece::setName(char value) {
	this->name = value;
}

void Piece::setPosn(const Posn &posn) {
	this->posn = posn;
}

void Piece::setX(unsigned int posX) {
	this->posn.x = posX;
}

void Piece::setY(unsigned int posY) {
	this->posn.y = posY;
}

std::ostream& operator<<(std::ostream &out, const Piece &posn) {
	out << posn.getName();
	return out;
}
