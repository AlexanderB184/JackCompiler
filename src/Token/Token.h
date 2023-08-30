#pragma once

#include <string>

namespace Jack {

struct Token {
  enum class Type {
    None = 0,
    Keyword,
    Identifier,
    Number,
    String,
    Symbol,
    Invalid
  };
  std::string value;
  Token::Type type;
  std::size_t row, col;
  Token(std::string&& str, Token::Type type, std::size_t line, std::size_t col);
  Token(std::string const& str, Token::Type type, std::size_t line,
        std::size_t col);
  Token(const Token&);
  Token(Token&&);
  Token();
  Token& operator=(const Token&);
  Token& operator=(Token&&);
};

}  // namespace Jack