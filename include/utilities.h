// include/utilities.h
#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

const unsigned int WIDTH = 8; // Board size; if we wanted to implement a custom
const unsigned int HEIGHT = 8; // sized board all we have to do is change these

struct Posn {
    unsigned int x, y;
    Posn(unsigned int x, unsigned int y);
    Posn(const std::string &pos);
    bool validate() const;
    bool operator==(const Posn &other) const;
};

struct BadPosn: public exception {
    Posn posn;
    BadPosn(const Posn &posn);
};

struct Move {
    Posn oldPos, newPos;
    Move(Posn o = {0, 0}, Posn n = {0, 0});
    bool operator==(const Move &other) const; // This one might be unnecessary, can remove if we don't use it
};

struct BadMove: public exception {
    Move move;
    BadMove(const Move &move);
};

#endif // UTILITIES_H
