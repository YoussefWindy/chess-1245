// src/ai.cc

#include "../include/ai.h"

Move empty = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): board{b}, white{w}, difficulty{d} {}

Move AI::think() const {
    switch (difficulty) {
        case 1:
            return thinkLevel1();
        case 2:
            return thinkLevel2();
        case 3:
            return thinkLevel3();
        case 4:
            return thinkLevel4();
        case 5:
            return thinkLevel5();
        default:
            return empty;
    }
}

Move AI::thinkLevel1() const {
    // Fill in
    return empty;
}

Move AI::thinkLevel2() const {
    // Fill in
    return empty;
}

Move AI::thinkLevel3() const {
    // Fill in
    return empty;
}

Move AI::thinkLevel4() const {
    // Fill in
    return empty;
}

Move AI::thinkLevel5() const {
    // Fill in
    return empty;
}
