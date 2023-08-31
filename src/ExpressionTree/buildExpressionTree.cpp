#include "StatementTree.h"
namespace Jack {

statement_ptr_t ExpressionTreeBuilder::buildStatements(const ParseTree& root) {
  const std::vector<ParseTree>& children = root.getChildren();
  auto statements = std::make_unique<Statements>();
  for (auto child : children) {
    statements->sequence.push_back(buildStatement(child));
  }
  return statements;
}

statement_ptr_t ExpressionTreeBuilder::buildStatement(const ParseTree& root) {
  switch (root.getType()) {
    case ParseTree::Type::Statements:
      return buildStatements(root);
    case ParseTree::Type::LetStatement:
      return buildLetStatement(root);
    case ParseTree::Type::DoStatement:
      return buildDoStatement(root);
    case ParseTree::Type::IfStatement:
      return buildIfStatement(root);
    case ParseTree::Type::WhileStatement:
      return buildWhileStatement(root);
    case ParseTree::Type::ReturnStatement:
      return buildReturnStatement(root);
    default:
      // Unreachable;
      break;
  }
}

statement_ptr_t ExpressionTreeBuilder::buildLetStatement(const ParseTree&) {}

statement_ptr_t ExpressionTreeBuilder::buildDoStatement(const ParseTree& root) {
  const std::vector<ParseTree>& children = root.getChildren();
  auto doStatement = std::make_unique<DoStatement>();
  doStatement->action = buildExpression(children[1]);
  return doStatement;
}

statement_ptr_t ExpressionTreeBuilder::buildIfStatement(const ParseTree&) {}

statement_ptr_t ExpressionTreeBuilder::buildWhileStatement(const ParseTree&) {}

statement_ptr_t ExpressionTreeBuilder::buildReturnStatement(
    const ParseTree& root) {
  const std::vector<ParseTree>& children = root.getChildren();
  auto retStatement = std::make_unique<ReturnStatement>();
  retStatement->returnValue = buildExpression(children[1]);
  return retStatement;
}

expression_ptr_t ExpressionTreeBuilder::buildExpression(const ParseTree&) {}

}  // namespace Jack
