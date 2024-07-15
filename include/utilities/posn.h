// include/utilities/posn.h
#ifndef POSN_H
#define POSN_H

 #include <string>

struct Posn {
    unsigned int x, y;
    Posn(unsigned int x, unsigned int y);
    Posn(const std::string &pos);
    bool validate() const;
    bool operator==(const Posn &other) const;
};

#endif // POSN_H
