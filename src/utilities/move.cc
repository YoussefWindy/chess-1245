// src/utilities/move.cc

#include "../../include/utilities.h"

Move::Move(Posn o, Posn n): oldPos{o}, newPos{n} {}

bool Move::operator==(const Move &other) const {
	return oldPos == other.oldPos && newPos == other.newPos;
}
