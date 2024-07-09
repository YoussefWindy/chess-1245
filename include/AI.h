#include "board.h"

class AI {
    Board &board;
    bool white;
    int difficulty;
    Move think1() const;
    Move think2() const;
    Move think3() const;
    Move think4() const;
    Move think5() const;
  public:
    AI(Board &board, bool colour, int difficulty);
    Move think() const;
};
