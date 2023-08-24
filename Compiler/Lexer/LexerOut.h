#pragma once

#include <iostream>

#include "Lexer.h"

namespace Jack {

std::string to_string(Jack::TokenType);
std::string to_string(const Jack::Token&);
std::string to_string(const std::vector<Jack::Token>&);
std::ostream& operator<<(std::ostream&, Jack::TokenType);
std::ostream& operator<<(std::ostream&, const Jack::Token&);
std::ostream& operator<<(std::ostream&, const std::vector<Jack::Token>&);

}  // namespace Jack
