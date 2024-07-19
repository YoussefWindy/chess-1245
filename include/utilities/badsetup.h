// include/utilities/badsetup.h
#ifndef BADSETUP_H
#define BADSETUP_H

#include <exception>
#include <string>

struct BadSetup: public std::exception {
  private:
    const std::string reason;
  public:
    BadSetup(const std::string &reason);
    const std::string why() const;
};

#endif // BADSETUP_H
