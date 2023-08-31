#pragma once

#include <vector>

#include "../ParseTree/ParseTree.h"
#include "../SymbolTable/SymbolTable.h"
#include "ExpressionTree.h"

namespace Jack {

typedef std::unique_ptr<Statement> statement_ptr_t;
typedef std::unique_ptr<Expression> expression_ptr_t;
typedef std::unique_ptr<LValueExpression> l_expression_ptr_t;


class ExpressionTreeBuilder {

  static statement_ptr_t buildStatements(const ParseTree&);
  static statement_ptr_t buildStatement(const ParseTree&);
  static statement_ptr_t buildLetStatement(const ParseTree&);
  static statement_ptr_t buildDoStatement(const ParseTree&);
  static statement_ptr_t buildIfStatement(const ParseTree&);
  static statement_ptr_t buildWhileStatement(const ParseTree&);
  static statement_ptr_t buildReturnStatement(const ParseTree&);

  static expression_ptr_t buildExpression(const ParseTree&);
};

}  // namespace Jack