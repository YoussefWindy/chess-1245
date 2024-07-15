// include/ai.h
#ifndef AI_H
#define AI_H

#include "board.h"

class AI {
    Board &board;
    bool white;
    int difficulty;
    Move thinkLevel1() const;
    Move thinkLevel2() const;
    Move thinkLevel3() const;
    Move thinkLevel4() const;
    Move thinkLevel5() const;
  public:
    AI(Board &board, bool colour, int difficulty);
    Move think() const;
};

#endif // AI_H
