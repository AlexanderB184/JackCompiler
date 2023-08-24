#include "LexerOut.h"
namespace Jack {

std::string to_string(Jack::TokenType type) {
  switch (type) {
    case Jack::TokenType::None:
      return "None";
    case Jack::TokenType::Keyword:
      return "Keyword";
    case Jack::TokenType::Identifier:
      return "Identifier";
    case Jack::TokenType::Number:
      return "Number";
    case Jack::TokenType::String:
      return "String";
    case Jack::TokenType::Symbol:
      return "Symbol";
    case Jack::TokenType::Invalid:
      return "Invalid";
    default:
      return "Unreachable";
  }
}
std::string to_string(const Jack::Token& tok) {
  return tok.value;
}

std::string to_string(const std::vector<Jack::Token>& tokens) {
  std::string str = "[";
  for (size_t i = 0; i < tokens.size(); i++) {
    str += to_string(tokens.at(i));
    if (i != tokens.size() - 1) {
      str += ", ";
    }
  }
  str += "]";
  return str;
}

std::ostream& operator<<(std::ostream& o, Jack::TokenType type) {
  o << to_string(type);
  return o;
}
std::ostream& operator<<(std::ostream& o, const Jack::Token& token) {
  o << to_string(token);
  return o;
}
std::ostream& operator<<(std::ostream& o,
                         const std::vector<Jack::Token>& tokens) {
  o << to_string(tokens);
  return o;
}

}  // namespace Jack