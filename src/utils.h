#ifndef PARSER_SRC_UTILS_H_
#define PARSER_SRC_UTILS_H_

#include <string>
#include <vector>

#include "nodes.h"

namespace SimpleParser {

int8_t getPriority(char);
std::vector<std::string> lexer(std::string);

}

#endif  // PARSER_SRC_UTILS_H_
