// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, bool colour, int value, const Posn &posn, bool v, bool h, bool p, bool n):
  name{name}, colour{colour}, value{value}, posn{posn}, vertical{v},
  horizontal{h}, positive{p}, negative{n}, hasMoved{false}, isProtected{false} {}

void Piece::intersect(std::vector<Posn> &positions) {
	for (auto it = legalMoves.begin(); it != legalMoves.end(); it++) {
		bool found = false;
		for (auto p: positions) {
			if (p == *it) {
				found = true;
				break;
			}
		}
		if (!found) {
			legalMoves.erase(it);
			it--;
		}
	}
}

bool Piece::canMoveTo(const Posn &posn) const {
	for (Posn p: legalMoves) {
		if (p == posn) return true;
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

int Piece::getValue() const {
	return value;
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

const std::vector<Posn>& Piece::getLegalMoves() const {
	return legalMoves;
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

void Piece::pin(bool vertical, bool horizontal, bool positive, bool negative) {
	if (this->vertical) this->vertical = vertical;
	if (this->horizontal) this->horizontal = horizontal;
	if (this->positive) this->positive = positive;
	if (this->negative) this->negative = negative;
}

void Piece::protect(bool isProtected) {
	this->isProtected = isProtected;
}

inline std::ostream& operator<<(std::ostream &out, const Piece &posn) {
	out << posn.getName();
	return out;
}
