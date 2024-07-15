// src/utilities/posn.cc

#include "../../include/utilities.h"

Posn::Posn(unsigned int x, unsigned int y): x{x}, y{y} {
	if (!validate()) throw BadPosn{*this};
}

Posn::Posn(const std::string &pos) {
	try {
		x = pos[0] - 'a';
		y = pos[1] - '1';
	} catch (...) {
		throw BadPosn{{WIDTH, HEIGHT}};
	}
	if (!validate()) throw BadPosn{*this};
}

bool Posn::validate() const {
	return 0 <= x && x < WIDTH  && 0 <= y && y < HEIGHT;
}

bool Posn::operator==(const Posn &other) const {
	return x == other.x && y == other.y;
}
