// src/utilities/move.cc

#include "../../include/utilities.h"

Move::Move(Posn o, Posn n, bool c, unsigned int p):
  oldPos{o}, newPos{n}, capture{c}, promotion{p} {}

bool Move::operator==(const Move &other) const {
	return oldPos == other.oldPos && newPos == other.newPos
		&& capture == other.capture && promotion == other.promotion;
}
