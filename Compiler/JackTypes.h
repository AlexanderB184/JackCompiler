#pragma once

#include <string>
#include <unordered_set>
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
  std::size_t row, col;
  Token(std::string&& str, TokenType type, std::size_t line, std::size_t col);
  Token(std::string const& str, TokenType type, std::size_t line, std::size_t col);
};

enum class ParseTreeType {
  Class,
  ClassVarDec,
  Subroutine,
  ParameterList,
  SubroutineBody,
  VarDec,
  Statements,
  LetStatement,
  DoStatement,
  IfStatement,
  WhileStatement,
  ReturnStatement,
  Expression,
  Term,
  ExpressionList,
  Keyword,
  Identifier,
  Number,
  String,
  Symbol
};

struct ParseTree {
  ParseTreeType type;
  std::vector<ParseTree> children;
  Token tok;
  ParseTree(Token);
  ParseTree(ParseTreeType);
};

}  // namespace Jack
