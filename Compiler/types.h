#pragma once

#include <string>
#include <vector>

namespace Jack {
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
  std::string value;
  TokenType type;
  size_t row, col;
  Token(std::string&&, TokenType, size_t, size_t);
  Token(std::string const&, TokenType, size_t, size_t);
};

enum class ParseTreeType {};

struct ParseTree {
  ParseTreeType type;
  std::vector<ParseTree> children;
  Token tok;
};

}  // namespace Jack
