// src/utilities.cc

#include "../include/utilities.h"
#include <memory>

Posn::Posn(unsigned int x, unsigned int y): x{x}, y{y} {}

Posn::Posn(const std::string &pos): x{pos[0] - 'a'}, y{pos[1] - '1'} {}

bool Posn::validate() const {
	return 0 <= x && x < WIDTH  && 0 <= y && y < HEIGHT;
}

bool Posn::operator==(const Posn &other) const {
	return x == other.x && y == other.y;
}

Move::Move(Posn o, Posn n): oldPos{o}, newPos{n} {}

bool Move::validate() const {
	return oldPos.validate() && newPos.validate();
}

bool Move::operator==(const Move &other) const {
	return oldPos == other.oldPos && newPos == other.newPos;
}
