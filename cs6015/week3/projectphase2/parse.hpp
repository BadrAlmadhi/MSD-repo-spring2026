#ifndef PARSE_H
#define PARSE_H

#include <istream>   // use istream, not iostream
#include <string>
#include "expr.hpp"
#include "pointer.h"

// Parse one expression from the stream.
// After parsing it checks that there is no extra junk.
PTR(Expr) parse(std::istream &in);

// Helper for parsing from a string (used in tests)
PTR(Expr) parse_str(const std::string &s);

#endif