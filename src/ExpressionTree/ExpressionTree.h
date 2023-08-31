#pragma once

#include <memory>
#include <vector>

#include "../ParseTree/ParseTree.h"
#include "../SymbolTable/SymbolTable.h"

namespace Jack {

typedef std::unique_ptr<Statement> statement_ptr_t;
typedef std::unique_ptr<Expression> expression_ptr_t;
typedef std::unique_ptr<LValueExpression> l_expression_ptr_t;

class Node {
 public:
  friend class ExpressionTreeBuilder;
  virtual std::string& generateCode() = 0;
};

class Statement : public Node {};

class Statements : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  std::vector<statement_ptr_t> sequence;

 public:
  std::string& generateCode();
};

class LetStatement : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  l_expression_ptr_t lhs;
  expression_ptr_t rhs;

 public:
  std::string& generateCode();
};

class DoStatement : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  expression_ptr_t action;

 public:
  std::string& generateCode();
};

class IfStatement : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  expression_ptr_t condition;
  statement_ptr_t trueBranch;
  statement_ptr_t falseBranch;

 public:
  std::string& generateCode();
};

class WhileStatement : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  expression_ptr_t condition;
  statement_ptr_t loopBody;

 public:
  std::string& generateCode();
};

class ReturnStatement : public Statement {
  friend class ExpressionTreeBuilder;

 private:
  expression_ptr_t returnValue;

 public:
  std::string& generateCode();
};

class Expression : public Node {};

class FunctionCall : public Expression {
  friend class ExpressionTreeBuilder;

 private:
  std::string Class;
  std::string Function;
  std::vector<expression_ptr_t> args;

 public:
  std::string& generateCode();
};

class BinaryExpression : public Expression {
  friend class ExpressionTreeBuilder;

 private:
  enum class Operation { Add, Sub, And, Or, Eq, Gt, Lt } op;
  expression_ptr_t lhs;
  expression_ptr_t rhs;

 public:
  std::string& generateCode();
};

class UnaryExpression : public Expression {
  friend class ExpressionTreeBuilder;

 private:
  enum class Operation { Neg, Not } op;
  expression_ptr_t term;

 public:
  std::string& generateCode();
};

class intConstant : public Expression {
  friend class ExpressionTreeBuilder;

 private:
  u_int16_t number;

 public:
  std::string& generateCode();
};

class stringConstant : public Expression {
  friend class ExpressionTreeBuilder;

 private:
  std::string str;

 public:
  std::string& generateCode();
};

class LValueExpression : public Expression {
 public:
  virtual std::string& generateLCode() = 0;
};

class variable : public LValueExpression {
  friend class ExpressionTreeBuilder;

 private:
  Symbol var;

 public:
  std::string& generateCode();
};

class arrayAccess : public LValueExpression {
  friend class ExpressionTreeBuilder;

 private:
  Symbol var;
  expression_ptr_t index;

 public:
  std::string& generateCode();
};

}  // namespace Jack
