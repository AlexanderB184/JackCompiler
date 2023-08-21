#pragma once

#include <string>
#include <vector>

namespace Jack {

using std::string;
using std::vector;

enum class TokenType {
  None = 0,
  Keyword,
  Identifier,
  Number,
  String,
  Symbol,
  Invalid
};

struct Token {
  string value;
  TokenType type;
  size_t row, col;
};

std::vector<Token> tokenize(const string&);

}  // namespace Jack
