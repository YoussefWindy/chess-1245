// src/utilities/move.cc

#include "../../include/utilities.h"

Move::Move(Posn o, Posn n, unsigned int p, bool c):
  oldPos{o}, newPos{n}, promotion{p}, capture{c} {}

bool Move::operator==(const Move &other) const {
	return oldPos == other.oldPos && newPos == other.newPos
		&& capture == other.capture && promotion == other.promotion;
}
