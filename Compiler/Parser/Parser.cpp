#include "Parser.h"

#define keyword TokenType::Keyword
#define identifier TokenType::Identifier
#define symbol TokenType::Symbol
#define stringConstant TokenType::String
#define intConstant TokenType::Number

#define next() tokptr++
#define have(TYPE, VAL) (tokptr->type == TYPE && tokptr->value == VAL)
#define couldBe(TYPE, VAL) \
  if (have(TYPE, VAL)) {   \
    addChild(*tokptr);     \
    next();                \
  }
#define couldBeType(TYPE) \
  if (haveType(TYPE)) {   \
    addChild(*tokptr);    \
    next();               \
  }
#define orCouldBe(TYPE, VAL)  \
  else if (have(TYPE, VAL)) { \
    addChild(*tokptr);        \
    next();                   \
  }
#define orCouldBeType(TYPE)  \
  else if (haveType(TYPE)) { \
    addChild(*tokptr);       \
    next();                  \
  }
#define otherwise(_STATEMENTS) \
  else {                       \
    _STATEMENTS                \
  }
#define otherwiseMustBe(TYPE, VAL) else mustBe(TYPE, VAL)
#define otherwiseMustBeType(TYPE) else mustBeType(TYPE)
#define except throwError("Parser Error", TYPE, *tokptr);
#define mustBe(TYPE, VAL)                           \
  if (have(TYPE, VAL)) {                            \
    addChild(*tokptr);                              \
    next();                                         \
  } else {                                          \
    throwError("Parser Error", TYPE, VAL, *tokptr); \
  }
#define haveType(TYPE) (tokptr->type == TYPE)
#define mustBeType(TYPE)                       \
  if (haveType(TYPE)) {                        \
    addChild(*tokptr);                         \
    next();                                    \
  } else {                                     \
    throwError("Parser Error", TYPE, *tokptr); \
  }

#define mustBeTypeSpecifier                          \
  couldBe(keyword, "int") orCouldBe(keyword, "char") \
      orCouldBe(keyword, "boolean") otherwiseMustBeType(identifier);
#define mustBeReturnTypeSpecifier                              \
  couldBe(keyword, "int") orCouldBe(keyword, "char")           \
      orCouldBe(keyword, "boolean") orCouldBe(keyword, "void") \
          otherwiseMustBeType(identifier);

#define addChild(tok) self.children.emplace_back(tok);

#define throwError(x...)
#define mustBeOp                                                              \
  if (tokptr->value == "+" || tokptr->value == "-" || tokptr->value == "*" || \
      tokptr->value == "/" || tokptr->value == "<" || tokptr->value == ">" || \
      tokptr->value == "=" || tokptr->value == "|" || tokptr->value == "&") { \
    addChild(*tokptr);                                                        \
    next();                                                                   \
  } else {                                                                    \
    except                                                                    \
  }
namespace Jack {

ParseTree parseProgram(Token* tokptr) {
  if (tokptr != nullptr) {
    return parseClass(tokptr);
  }
  throwError("Empty")
}

ParseTree parseClass(Token* tokptr) {
  ParseTree self(ParseTreeType::Class);
  mustBe(keyword, "class");
  mustBeType(identifier);
  mustBe(symbol, "{");
  while (have(keyword, "field") | have(keyword, "static")) {
    parseClassVarDec(self, tokptr);
  }
  while (have(keyword, "constructor") | have(keyword, "function") |
         have(keyword, "method")) {
    parseSubroutine(self, tokptr);
  }
}

int parseClassVarDec(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::ClassVarDec);
  ParseTree& self = parent.children.back();
  couldBe(keyword, "static") otherwiseMustBe(keyword, "field");
  mustBeTypeSpecifier;
  mustBeType(identifier);
  while (have(symbol, ",")) {
    mustBe(symbol, ",");
    mustBeType(identifier);
  }
  mustBe(symbol, ";");
}
int parseSubroutine(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::Subroutine);
  ParseTree& self = parent.children.back();
  couldBe(keyword, "constructor") orCouldBe(keyword, "method")
      otherwiseMustBe(keyword, "function");
  mustBeReturnTypeSpecifier;
  mustBeType(identifier);
  mustBe(symbol, "(");
  parseParameterList(self, tokptr);
  mustBe(symbol, ")");
  mustBe(symbol, "{");
  parseSubroutineBody(self, tokptr);
  mustBe(symbol, "}");
}

int parseParameterList(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::ParameterList);
  ParseTree& self = parent.children.back();
  couldBe(keyword, "int") orCouldBe(keyword, "char")
      orCouldBe(keyword, "boolean") orCouldBeType(identifier)
          otherwise(couldBe(keyword, "void") return 0;);
  couldBeType(identifier);
  while (have(symbol, ",")) {
    mustBe(symbol, ",");
    couldBe(keyword, "int") orCouldBe(keyword, "char")
        orCouldBe(keyword, "boolean") otherwiseMustBeType(identifier);
    mustBeType(identifier);
  }
  return 0;
}

int parseSubroutineBody(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::SubroutineBody);
  ParseTree& self = parent.children.back();
  while (have(keyword, "var")) {
    parseVarDec(self, tokptr);
  }
  parseStatements(self, tokptr);
}

int parseVarDec(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::VarDec);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "var");
  mustBeTypeSpecifier;
  mustBeType(identifier);
  while (have(symbol, ",")) {
    mustBe(symbol, ",");
    mustBeType(identifier);
  }
  mustBe(symbol, ";");
}

int parseStatements(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::Statements);
  ParseTree& self = parent.children.back();
}

int parseLetStatement(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::LetStatement);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "let");
  mustBeType(identifier);
  if (have(symbol, "[")) {
    mustBe(symbol, "[");
    parseExpression(self, tokptr);
    mustBe(symbol, "]");
  }
  mustBe(symbol, "=");
  parseExpression(self, tokptr);
  mustBe(symbol, ";");
}

int parseDoStatement(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::DoStatement);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "do");
  parseExpression(self, tokptr);
  mustBe(symbol, ";");
}

int parseIfStatement(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::IfStatement);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "if");
  mustBe(symbol, "(");
  parseExpression(self, tokptr);
  mustBe(symbol, ")");
  mustBe(symbol, "{");
  parseStatements(self, tokptr);
  mustBe(symbol, "}");
  if (have(keyword, "else")) {
    mustBe(keyword, "else");
    mustBe(symbol, "{");
    parseStatements(self, tokptr);
    mustBe(symbol, "}");
  }
}

int parseWhileStatement(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::WhileStatement);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "while");
  mustBe(symbol, "(");
  parseExpression(self, tokptr);
  mustBe(symbol, ")");
  mustBe(symbol, "{");
  parseStatements(self, tokptr);
  mustBe(symbol, "}");
}

int parseReturnStatement(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::ReturnStatement);
  ParseTree& self = parent.children.back();
  mustBe(keyword, "return");
  parseExpression(self, tokptr);
  mustBe(symbol, ";")
}

int parseExpression(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::Expression);
  ParseTree& self = parent.children.back();
  parseTerm(self, tokptr);
  if (haveType(symbol)) {
    mustBeOp;
    parseTerm(self, tokptr);
  }
}

int parseExpressionList(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::ExpressionList);
  ParseTree& self = parent.children.back();
  parseExpression(self, tokptr);
  while (have(symbol, ",")) {
    mustBe(symbol, ",");
    parseExpression(self, tokptr);
  }
}

int parseTerm(ParseTree& parent, Token*& tokptr) {
  parent.children.emplace_back(ParseTreeType::Term);
  ParseTree& self = parent.children.back();
  switch (tokptr->type) {
    case intConstant:
      mustBeType(intConstant);
      break;
    case stringConstant:
      mustBeType(intConstant);
      break;
    case keyword:
      couldBe(keyword, "this") orCouldBe(keyword, "null")
          orCouldBe(keyword, "true") otherwiseMustBe(keyword, "false");
      break;
    case symbol:
      if (have(symbol, "(")) {
        mustBe(symbol, "(");
        parseExpression(self, tokptr);
        mustBe(symbol, ")");
        break;
      }
      couldBe(symbol, "-") orCouldBe(symbol, "~");
      parseTerm(self, tokptr);
      break;
    case identifier:
      mustBeType(identifier);
      if (have(symbol, ".")) {
        mustBe(symbol, ".");
        mustBeType(identifier);
      }
      if (have(symbol, "(")) {
        mustBe(symbol, "(");
        parseExpressionList(self, tokptr);
        mustBe(symbol, ")");
      }
      if (have(symbol, "[")) {
        mustBe(symbol, "[");
        parseExpression(self, tokptr);
        mustBe(symbol, "]");
      }
      break;
  }
}

ParseTree parse(std::vector<Token>& tokens) {
  if (tokens.empty()) {
    except;
  }
  return parseProgram(tokens.data());
}
}  // namespace Jack
