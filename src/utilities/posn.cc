// src/utilities/posn.cc

#include "../../include/utilities.h"

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
