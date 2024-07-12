// src/utilities.cc

#include "../include/utilities.h"
#include <memory>

Posn::Posn(unsigned int x, unsigned int y): x{x}, y{y} {
	if (!validate()) throw BadPosn{*this};
}

Posn::Posn(const std::string &pos): x{pos[0] - 'a'}, y{pos[1] - '1'} {
	if (!validate()) throw BadPosn{*this};
}

bool Posn::validate() const {
	return 0 <= x && x < WIDTH  && 0 <= y && y < HEIGHT;
}

bool Posn::operator==(const Posn &other) const {
	return x == other.x && y == other.y;
}

Move::Move(Posn o, Posn n): oldPos{o}, newPos{n} {}

bool Move::operator==(const Move &other) const {
	return oldPos == other.oldPos && newPos == other.newPos;
}

BadPosn::BadPosn(const Posn &posn): posn{posn} {}

BadMove::BadMove(const Move &move): move{move} {}
