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
  Token(string&&, TokenType, size_t, size_t);
  Token(string const&, TokenType, size_t, size_t);
};

class Lexer {
 private:
  size_t curr_line, curr_col;
  string::const_iterator it;
  const string::const_iterator end;

  void next();
  void newline();
  void skip(int n);

  Token makeToken(TokenType);

  
  Token extractWord();
  Token extractNumber();
  Token extractString();
  Token extractSymbol();
  Token extractWhiteSpace();
  Token extractSingleLineComment();
  Token extractMultiLineComment();

 public:
  Lexer(string const&);
  Token extractToken();
  bool atEnd();
};
vector<Token> tokenize(const string&);
vector<Token> tokenize(const char*);
//vector<Token> tokenize(const string&);
void throwError(string, Token);

}  // namespace Jack
