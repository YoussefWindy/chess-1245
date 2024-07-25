// src/piece.cc

#include "../include/piece.h"

Piece::Piece(char name, bool colour, unsigned int value, const Posn &posn, bool v, bool h, bool p, bool n):
  name{name}, colour{colour}, isProtected{false}, value{value}, numMoves{0},
  posn{posn}, vertical{v}, horizontal{h}, positive{p}, negative{n} {}

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
	// std::cerr << "Legal moves of " << name << ": ";
	// for (auto p: legalMoves) {
	// 	std::cerr << char('a' + p.x) << p.y + 1 << ", ";
	// }
	// std::cerr << std::endl << "Therefore, legalMoves is " << (legalMoves.empty() ? "empty" : "not empty") << std::endl;
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

// const std::vector<Posn>& Piece::getLegalMoves() const {
// 	return legalMoves;
// }

bool Piece::getHasMoved() const {
	return numMoves;
}

bool Piece::getIsProtected() const {
	return isProtected;
}

// Setters
void Piece::move(const Posn &posn, bool forward) {
	numMoves += (forward ? 1 : -1);
	std::cerr << name << " has moved " << numMoves << " times." << std::endl;
	this->posn = posn;
}

void Piece::setName(char value) {
	this->name = value;
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

std::strong_ordering Piece::operator<=>(const Piece &other) const {
	auto result = value <=> other.value;
	if (result != 0) return result;
	result = posn.x <=> other.posn.x;
	if (result != 0) return result;
	return posn.y <=> other.posn.y;
}

inline std::ostream& operator<<(std::ostream &out, const Piece &piece) {
	out << piece.getName();
	return out;
}
