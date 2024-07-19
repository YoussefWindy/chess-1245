// src/utilities/badsetup.cc

#include "../../include/utilities/badsetup.h"

BadSetup::BadSetup(const std::string &reason): reason{reason} {}

const std::string BadSetup::why() const {
    return reason;
}
