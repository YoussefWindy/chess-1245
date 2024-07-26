// include/ai.h
#ifndef AI_H
#define AI_H

#include "board.h"

// The AI class is all-knowing.
// It has access to all private Board and Piece
// fields and methods in order to make decisions.
class AI {
    const Board &board;
    const bool colour;
    const int difficulty;
    mutable std::vector<Move> checkingMoves, capturingMoves;
    mutable std::vector<Posn> threatenedPosns;
    const std::vector<Move> calculateCheckingMoves(bool checkmatee) const;
    const std::vector<Move> calculateCapturingMoves() const;
    const std::vector<Posn> calculateThreatenedPosns() const;
    Move thinkAt1() const;
    Move thinkAt2() const;
    Move thinkAt3() const;
    Move thinkAt4() const;
  public:
    AI(Board &board, bool colour, int difficulty);
    Move think() const;
};

#endif // AI_H
