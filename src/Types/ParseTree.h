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

 private:
  const Type type;
  std::vector<ParseTree> children;
  Token tok;

 public:
  ParseTree(const Token&);
  ParseTree(ParseTree::Type);
  ParseTree(const ParseTree&);
  ParseTree(ParseTree&&);

  ParseTree& addChild(ParseTree::Type);
  ParseTree& addChild(const Token&);

  const std::vector<ParseTree>& getChildren() const;
  const std::string& getValue() const;
  size_t getLine() const;
  size_t getCol() const;
  ParseTree::Type getType() const;
};

}  // namespace Jack
