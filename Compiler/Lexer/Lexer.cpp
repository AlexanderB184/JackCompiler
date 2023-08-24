#include "Lexer.h"

#include "../CompileError/Error.h"
#include "LexerTokenClassification.h"

namespace Jack {

Lexer::Lexer(string const& script)
    : curr_line(0), curr_col(0), it(script.cbegin()), end(script.cend()) {}

void Lexer::next() {
  curr_col++;
  it++;
}

void Lexer::newline() {
  curr_col = 0;
  curr_line++;
}

void Lexer::skip(int n) {
  curr_col += n;
  it += n;
}

bool Lexer::atEnd() { return it >= end; }
Token Lexer::makeToken(TokenType type) {
  return Token{"", type, curr_line, curr_col};
}

Token Lexer::extractWord() {
  Token word = makeToken(TokenType::Identifier);
  while (it != end && !isWhiteSpace(*it) && !isSymbol(*it)) {
    word.value.push_back(*it);
    next();
  }
  if (isKeyword(word.value)) word.type = TokenType::Keyword;
  return word;
}

Token Lexer::extractNumber() {
  Token number = makeToken(TokenType::Number);
  while (it != end && !isWhiteSpace(*it) && !isSymbol(*it) && !isLetter(*it)) {
    number.value.push_back(*it);
    next();
  }
  return number;
}

Token Lexer::extractSymbol() {
  Token symbol = makeToken(TokenType::Symbol);
  symbol.value.push_back(*it);
  next();
  return symbol;
}

Token Lexer::extractString() {
  Token str = makeToken(TokenType::String);
  skip(1);
  while (it < end && (*it) != '\"' && (*it) != '\n' && (*it) != '\r') {
    str.value.push_back(*it);
    next();
  }
  if (it != end) {
    skip(1);
  } else {
    throwError("unclosed quotes", str);
  }
  return str;
}

Token Lexer::extractWhiteSpace() {
  Token none = makeToken(TokenType::None);
  while (it < end && isWhiteSpace(*it)) {
    if (*it == '\n' || *it == '\r')
      newline();
    else
      next();
  }
  return none;
}

Token Lexer::extractSingleLineComment() {
  Token none = makeToken(TokenType::None);
  skip(2);
  while (it < end && *it != '\n' && *it != '\r') {
    next();
  }
  if (it < end) next();
  return none;
}

Token Lexer::extractMultiLineComment() {
  Token none = makeToken(TokenType::None);
  skip(2);
  while (it < end) {
    if (*it == '*' && it + 1 < end && *(it + 1) == '/') {
      skip(2);
      return none;
    }
    next();
  }
  throwError("unclosed multi line comment", none);
}

Token Lexer::extractToken() {
  if (isWhiteSpace(*it)) return extractWhiteSpace();
  if (isLetter(*it)) return extractWord();
  if (isNumber(*it)) return extractNumber();
  if (*it == '\"') return extractString();
  if (*it == '/' && *(it + 1) == '/') return extractSingleLineComment();
  if (*it == '/' && *(it + 1) == '*') return extractMultiLineComment();
  if (isSymbol(*it)) return extractSymbol();
  throwError("unexpected symbol", makeToken(TokenType::Invalid));
}

vector<Token> tokenize(const string& script) {
  Lexer lexer(script);
  vector<Token> tokens;
  while (lexer.atEnd()) {
    Token token = lexer.extractToken();
    if (token.type != TokenType::None) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

void throwError(string type, Token what) { return; }

}  // namespace Jack
