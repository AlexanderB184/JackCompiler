#include "LexerTokenClassification.h"

#include "Lexer.h"

namespace Jack {

bool isWhiteSpace(char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool isNumber(char c) { return c >= '0' && c <= '9'; }

bool isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isSymbol(char c) {
  return c == '&' || c == '|' || c == '=' || c == '<' || c == '>' || c == '+' ||
         c == '-' || c == '*' || c == '/' || c == '.' || c == ',' || c == '(' ||
         c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '~' ||
         c == ';';
}

bool isKeyword(const string& word) {
  static unordered_set<string> keywords(
      {"class", "function", "method", "constructor", "static", "field",
       "var",   "return",   "while",  "if",          "else",   "let",
       "do",    "int",      "char",   "boolean",     "void",   "true",
       "false", "null",     "this"});
  return (keywords.find(word) != keywords.end());
}

TokenType classifyToken(char c) {
  if (isWhiteSpace(c)) return TokenType::None;
  if (isLetter(c)) return TokenType::Identifier;
  if (isNumber(c)) return TokenType::Number;
  if (c == '\"') return TokenType::String;
  if (isSymbol(c)) return TokenType::Symbol;
  return TokenType::Invalid;
}

}  // namespace Jack