#include "Lexer.h"

#include "LexerTokenClassification.h"

namespace Jack {

Token extractWord(string::const_iterator& it, string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  Token word = {"", TokenType::Identifier, curr_row, curr_col};
  while (it != end && !isWhiteSpace(*it) && !isSymbol(*it)) {
    word.value.push_back(*it);
    curr_col++;
    it++;
  }
  if (isKeyword(word.value)) word.type = TokenType::Keyword;
  return word;
}

Token extractNumber(string::const_iterator& it, string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  Token number = {"", TokenType::Number, curr_row, curr_col};
  while (it != end && !isWhiteSpace(*it) && !isSymbol(*it) && !isLetter(*it)) {
    number.value.push_back(*it);
    curr_col++;
    it++;
  }
  return number;
}

Token extractSymbol(string::const_iterator& it, string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  Token symbol = {"", TokenType::Symbol, curr_row, curr_col};
  symbol.value.push_back(*it);
  curr_col++;
  it++;
  return symbol;
}

/**
 * @todo Add error handling for unclosed string
 */
Token extractString(string::const_iterator& it, string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  Token str = {"", TokenType::String, curr_row, curr_col};
  it++;
  while (it != end && !isWhiteSpace(*it) && !isSymbol(*it) && !isLetter(*it)) {
    str.value.push_back(*it);
    curr_col++;
    it++;
  }
  if (it != end) {
    curr_col++;
    it++;
  } else { /* unclosed quotation marks */
    /* throw "unclosed quotation marks on line", str.row, "column", str.col;
    print line str.row;
    */
  }
  return str;
}

Token extractWhiteSpace(string::const_iterator& it,
                        string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  Token none = {"", TokenType::None, curr_row, curr_col};
  while (it != end && isWhiteSpace(*it)) {
    if (*it == '\r' || *it == '\n') {
      curr_col = 0;
      curr_row++;
    } else {
      curr_col++;
    }
    it++;
  }
  return none;
}

/**
 * @todo Add error handling for unknown character
 */
Token extractToken(string::const_iterator& it, string::const_iterator end) {
  static size_t curr_row = 0, curr_col = 0;
  TokenType type = classifyToken(*it);
  switch (type) {
    case TokenType::None:
      return extractWhiteSpace(it, end);
    case TokenType::Keyword:
      return extractWord(it, end);
    case TokenType::Identifier:
      return extractWord(it, end);
    case TokenType::Number:
      return extractNumber(it, end);
    case TokenType::String:
      return extractString(it, end);
    case TokenType::Symbol:
      return extractSymbol(it, end);
    case TokenType::Invalid:

      // Handle error

      return Token{"", TokenType::Invalid, curr_row, curr_col};
  }
}

/**
 * @todo Add error handling
 * @todo Add comment handling
 * @todo make tests
 */
vector<Token> tokenize(const string& script) {
  static size_t curr_row, curr_col;
  curr_row = 0;
  curr_col = 0;
  string::const_iterator it = script.cbegin();
  string::const_iterator end = script.cend();
  vector<Token> tokens;
  while (it != end) {
    Token token = extractToken(it, end);
    if (token.type == TokenType::Invalid) {
      // ERROR
    }
    if (token.type != TokenType::None) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

}  // namespace Jack
