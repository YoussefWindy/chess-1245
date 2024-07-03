// src/piece.cc

#include "../include/piece.h"
#include "../include/board.h"

Piece::Piece(char name, Board::Posn) : name{name}, posn{posn} {
	if ('b' <= name && name <= 'r') {
		colour = 0;
	} else {
		colour = 1;
	}
}

// Getter
char Piece::getName() const {
	return name;
}

bool Piece::getColour() const {
	return colour;
}

Board::Posn Piece::getPosn() const {
	return posn;
}

unsigned int Piece::getX() const {
	return posn.x;
}

unsigned int Piece::getY() const {
	return posn.y;
}

// Setter
void Piece::setName(char value) {
	this->name = value;
}

void Piece::setPosn(Board::Posn posn) {
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
