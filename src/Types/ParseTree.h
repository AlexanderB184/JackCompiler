#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Token.h"

namespace Jack {

struct ParseTree {
  enum class Type {
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
  Type type;
  std::vector<ParseTree> children;
  Token tok;
  ParseTree(const Token&);
  ParseTree(ParseTree::Type);
  ParseTree(const ParseTree&);
  ParseTree(ParseTree&&);
  ParseTree& operator=(const ParseTree&);
  ParseTree& operator=(ParseTree&&);
};

}  // namespace Jack
