#ifndef PARSER_SRC_PARSER_H_
#define PARSER_SRC_PARSER_H_

#include <string>

#include "tree.h"

namespace SimpleParser {

double calculate(std::string);
double calculate(std::string, const ConstantMap*);

std::string print(std::string);
std::string print(std::string, const ConstantMap*);

}

#endif  // PARSER_SRC_PARSER_H_
