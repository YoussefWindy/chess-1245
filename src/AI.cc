#include "../include/AI.h"

AI::AI(Board &b, bool w, int d): board{b}, white{w}, difficulty{d} {}

Move AI::think() const {
    switch (difficulty) {
        case 1:
            return think1();
        case 2:
            return think2();
        case 3:
            return think3();
        case 4:
            return think4();
        case 5:
            return think5();
    }
}

Move AI::think1() const {
    // Fill in
}

Move AI::think2() const {
    // Fill in
}

Move AI::think3() const {
    // Fill in
}

Move AI::think4() const {
    // Fill in
}

Move AI::think5() const {
    // Fill in
}
