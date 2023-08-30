#include "Token.h"

#include <string.h>

using std::string;

namespace Jack {

Token::Token() : value(), type(), row(), col() {}

Token::Token(string&& val, Token::Type type, size_t line, size_t col)
    : value(std::move(val)), type(type), row(line), col(col) {}

Token::Token(const string& val, Token::Type type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}
Token::Token(const Token& tok)
    : value(tok.value), type(tok.type), row(tok.row), col(tok.col) {}

Token::Token(Token&& tok)
    : value(std::move(tok.value)),
      type(std::move(tok.type)),
      row(std::move(tok.row)),
      col(std::move(tok.col)) {}

Token& Token::operator=(const Token& tok) {
  value = tok.value;
  type = tok.type;
  row = tok.row;
  col = tok.col;
  return *this;
}
Token& Token::operator=(Token&& tok) {
  value = std::move(tok.value);
  type = std::move(tok.type);
  row = std::move(tok.row);
  col = std::move(tok.col);
  return *this;
}

}  // namespace Jack