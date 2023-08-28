#include "Parser.h"

#include <iostream>

#include "../ParseTree.h"
namespace Jack {
#define OKAY()                                                        \
  return ParserResult(ParserResult::ExitCode::okay, Token(), Token(), \
                      tok->row, tok->col);
#define ERROR(EXPECTEDVALUE, EXPECTEDTYPE, ERRORCODE)                      \
  return ParserResult(                                                     \
      ParserResult::ExitCode::ERRORCODE, std::move(*tok),                  \
      Token{EXPECTEDVALUE, Token::Type::EXPECTEDTYPE, tok->row, tok->col}, \
      tok->row, tok->col);
#define COULD_BE(TYPE, VALUE)                                  \
  if (tok->type == Token::Type::TYPE && tok->value == VALUE) { \
    parent.children.emplace_back(*tok);                        \
    if (++tok == end) {                                        \
      ERROR(VALUE, TYPE, missing_token)                        \
    }                                                          \
  }
#define OTHERWISE_MUST_BE_TYPE(TYPE, msg) MUST_BE_TYPE(TYPE, msg)
#define HAVE(TYPE, VALUE) \
  (tok->type == Token::Type::TYPE && tok->value == VALUE)
#define HAVE_TYPE(TYPE) (tok->type == Token::Type::TYPE)
#define ADD(TYPE, VALUE)              \
  parent.children.emplace_back(*tok); \
  if (++tok == end) {                 \
    ERROR(VALUE, TYPE, missing_token) \
  }
#define MUST_BE(TYPE, VALUE)                                   \
  if (tok->type == Token::Type::TYPE && tok->value == VALUE) { \
    ADD(TYPE, VALUE)                                           \
  } else {                                                     \
    ERROR(VALUE, TYPE, unexpected_token)                       \
  }
#define MUST_BE_TYPE(TYPE, msg)         \
  if (tok->type == Token::Type::TYPE) { \
    ADD(TYPE, msg)                      \
  } else {                              \
    ERROR(msg, TYPE, unexpected_token)  \
  }
#define PARSE(TYPE)                                                       \
  parent.children.emplace_back(ParseTree::Type::TYPE);                    \
  ParserResult Res##TYPE = parse##TYPE(parent.children.back(), tok, end); \
  if (Res##TYPE.exit_code != ParserResult::ExitCode::okay) {              \
    return Res##TYPE;                                                     \
  }
ParserResult::ParserResult(ParserResult::ExitCode exit_code, ParseTree&& pt,
                           size_t line, size_t col)
    : exit_code(exit_code),
      abstractSyntaxTree(std::move(pt)),
      problemToken(),
      wantedToken(),
      exit_line(line),
      exit_col(col) {
  ;
}

ParserResult::ParserResult(ParserResult::ExitCode exit_code, const Token& tok,
                           const Token& expected, size_t line, size_t col)
    : exit_code(exit_code),
      abstractSyntaxTree(Token{"", Token::Type::None, 0, 0}),
      problemToken(tok),
      wantedToken(expected),
      exit_line(line),
      exit_col(col) {}

ParserResult parseClass(ParseTree& parent,
                        std::vector<Token>::const_iterator& tok,
                        std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "class")
  MUST_BE_TYPE(Identifier, "class name")
  MUST_BE(Symbol, "{")
  while (HAVE(Keyword, "field") || HAVE(Keyword, "static")) {
    PARSE(ClassVarDec)
  }
  while (HAVE(Keyword, "constructor") || HAVE(Keyword, "function") ||
         HAVE(Keyword, "method")) {
    PARSE(Subroutine)
  }
  MUST_BE(Symbol, "}")
  OKAY()
}

ParserResult parseClassVarDec(ParseTree& parent,
                              std::vector<Token>::const_iterator& tok,
                              std::vector<Token>::const_iterator const end) {
  COULD_BE(Keyword, "field")
  else MUST_BE(Keyword, "static");
  COULD_BE(Keyword, "int")
  else COULD_BE(Keyword, "char") else COULD_BE(
      Keyword, "boolean") else MUST_BE_TYPE(Identifier, "type specifier");
  MUST_BE_TYPE(Identifier, "variable name")
  while (HAVE(Symbol, ",")) {
    MUST_BE(Symbol, ",")
    MUST_BE_TYPE(Identifier, "variable name")
  }
  MUST_BE(Symbol, ";")
  OKAY()
}

ParserResult parseSubroutine(ParseTree& parent,
                             std::vector<Token>::const_iterator& tok,
                             std::vector<Token>::const_iterator const end) {
  COULD_BE(Keyword, "constructor")
  else COULD_BE(Keyword, "method") else MUST_BE(Keyword, "function");
  COULD_BE(Keyword, "int")
  else COULD_BE(Keyword, "char") else COULD_BE(
      Keyword,
      "boolean") else COULD_BE(Keyword,
                               "void") else MUST_BE_TYPE(Identifier,
                                                         "type specifier");
  MUST_BE_TYPE(Identifier, "function name")
  MUST_BE(Symbol, "(")
  PARSE(ParameterList)
  MUST_BE(Symbol, ")")
  MUST_BE(Symbol, "{")
  PARSE(SubroutineBody)
  MUST_BE(Symbol, "}")
  OKAY()
}

ParserResult parseParameterList(ParseTree& parent,
                                std::vector<Token>::const_iterator& tok,
                                std::vector<Token>::const_iterator const end) {
  if (!HAVE(Symbol, ")")) {
    COULD_BE(Keyword, "int")
    else COULD_BE(Keyword, "char") else COULD_BE(
        Keyword, "boolean") else MUST_BE_TYPE(Identifier, "type specifier");
    MUST_BE_TYPE(Identifier, "argument name")
  }
  while (HAVE(Symbol, ",")) {
    MUST_BE(Symbol, ",")
    COULD_BE(Keyword, "int")
    else COULD_BE(Keyword, "char") else COULD_BE(
        Keyword, "boolean") else MUST_BE_TYPE(Identifier, "type specifier");
    MUST_BE_TYPE(Identifier, "argument name")
  }
  OKAY()
}

ParserResult parseSubroutineBody(ParseTree& parent,
                                 std::vector<Token>::const_iterator& tok,
                                 std::vector<Token>::const_iterator const end) {
  while (HAVE(Keyword, "var")) {
    PARSE(VarDec)
  }
  PARSE(Statements)
  OKAY()
}

ParserResult parseVarDec(ParseTree& parent,
                         std::vector<Token>::const_iterator& tok,
                         std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "var");
  COULD_BE(Keyword, "int")
  else COULD_BE(Keyword, "char") else COULD_BE(
      Keyword, "boolean") else MUST_BE_TYPE(Identifier, "type specifier");

  while (HAVE(Symbol, ",")) {
    MUST_BE(Symbol, ",") MUST_BE_TYPE(Identifier, "variable name")
  }
  MUST_BE(Symbol, ";") OKAY()
}

ParserResult parseStatements(ParseTree& parent,
                             std::vector<Token>::const_iterator& tok,
                             std::vector<Token>::const_iterator const end) {
  while (!HAVE(Symbol, "}")) {
    if (HAVE(Keyword, "let")) {
      PARSE(LetStatement);
    } else if (HAVE(Keyword, "do")) {
      PARSE(DoStatement);
    } else if (HAVE(Keyword, "if")) {
      PARSE(IfStatement);
    } else if (HAVE(Keyword, "while")) {
      PARSE(WhileStatement);
    } else if (HAVE(Keyword, "return")) {
      PARSE(ReturnStatement);
    } else {
      ERROR("statement type", Keyword, unexpected_token)
    }
  }
  OKAY();
}

ParserResult parseLetStatement(ParseTree& parent,
                               std::vector<Token>::const_iterator& tok,
                               std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "let");
  MUST_BE_TYPE(Identifier, "variable name");
  if (HAVE(Symbol, "[")) {
    MUST_BE(Symbol, "[");
    PARSE(Expression)
    MUST_BE(Symbol, "]");
  }
  MUST_BE(Symbol, "=")
  PARSE(Expression)
  MUST_BE(Symbol, ";")
  OKAY()
}
ParserResult parseDoStatement(ParseTree& parent,
                              std::vector<Token>::const_iterator& tok,
                              std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "do");
  PARSE(Expression);
  MUST_BE(Symbol, ";");
  OKAY();
}
ParserResult parseIfStatement(ParseTree& parent,
                              std::vector<Token>::const_iterator& tok,
                              std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "if");
  MUST_BE(Symbol, "(");
  PARSE(Expression)
  MUST_BE(Symbol, ")");
  MUST_BE(Symbol, "{")
  PARSE(Statements)
  MUST_BE(Symbol, "}")
  if (HAVE(Keyword, "else")) {
    MUST_BE(Keyword, "else");
    MUST_BE(Symbol, "{")
    PARSE(Statements)
    MUST_BE(Symbol, "}")
  }
  OKAY()
}
ParserResult parseWhileStatement(ParseTree& parent,
                                 std::vector<Token>::const_iterator& tok,
                                 std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "while");
  MUST_BE(Symbol, "(");
  PARSE(Expression)
  MUST_BE(Symbol, ")");
  MUST_BE(Symbol, "{")
  PARSE(Statements)
  MUST_BE(Symbol, "}")
  OKAY()
}
ParserResult parseReturnStatement(
    ParseTree& parent, std::vector<Token>::const_iterator& tok,
    std::vector<Token>::const_iterator const end) {
  MUST_BE(Keyword, "return");
  PARSE(Expression);
  MUST_BE(Symbol, ";");
  OKAY();
}
ParserResult parseExpression(ParseTree& parent,
                             std::vector<Token>::const_iterator& tok,
                             std::vector<Token>::const_iterator const end) {
  PARSE(Term);
  while (tok->value == "+" || tok->value == "-" || tok->value == "*" ||
         tok->value == "/" || tok->value == "&" || tok->value == "|" ||
         tok->value == "=" || tok->value == "<" || tok->value == ">") {
    ADD(Symbol, "operation")
    PARSE(Term);
  }
  OKAY();
}

ParserResult parseExpressionList(ParseTree& parent,
                                 std::vector<Token>::const_iterator& tok,
                                 std::vector<Token>::const_iterator const end) {
  PARSE(Expression);
  while (HAVE(Symbol, ",")) {
    MUST_BE(Symbol, ",");
    PARSE(Expression);
  }
  OKAY();
}

ParserResult parseTerm(ParseTree& parent,
                       std::vector<Token>::const_iterator& tok,
                       std::vector<Token>::const_iterator const end) {
  switch (tok->type) {
    case Token::Type::Number:
      MUST_BE_TYPE(Number, "integer constant");
      break;
    case Token::Type::String:
      MUST_BE_TYPE(String, "string constant");
      break;
    case Token::Type::Keyword:
      COULD_BE(Keyword, "true")
      else COULD_BE(Keyword, "false") else COULD_BE(
          Keyword, "null") else MUST_BE(Keyword, "this");
      break;
    case Token::Type::Symbol:
      if (HAVE(Symbol, "(")) {
        MUST_BE(Symbol, "(");
        PARSE(Expression);
        MUST_BE(Symbol, ")");
      } else {
        COULD_BE(Symbol, "-") else MUST_BE(Symbol, "~");
        PARSE(Term);
      }
      break;
    case Token::Type::Identifier:
      MUST_BE_TYPE(Identifier, "variable name");
      if (HAVE(Symbol, "[")) {
        MUST_BE(Symbol, "[");
        PARSE(Expression);
        MUST_BE(Symbol, "]");
      } else if (HAVE(Symbol, "(")) {
        MUST_BE(Symbol, "(");
        PARSE(ExpressionList);
        MUST_BE(Symbol, ")");
      }
      if (HAVE(Symbol, ".")) {
        MUST_BE(Symbol, ".");
        MUST_BE_TYPE(Identifier, "function name");
        MUST_BE(Symbol, "(");
        PARSE(ExpressionList);
        MUST_BE(Symbol, ")");
      }
      break;
    default:
      ERROR("expected expression", Identifier, unexpected_token);
      break;
  }
  OKAY();
}

ParserResult parse(const std::vector<Token>& tokens) {
  ParseTree parsedTree(ParseTree::Type::Class);
  std::vector<Token>::const_iterator tok = tokens.cbegin();
  std::vector<Token>::const_iterator const end = tokens.cend();
  ParserResult res = parseClass(parsedTree, tok, end);
  if (res.exit_code != ParserResult::ExitCode::okay) {
    return res;
  }

  if (tok != end && tok->type != Token::Type::None) {
    return ParserResult(
        ParserResult::ExitCode::unexpected_token, std::move(*tok),
        Token{"", Token::Type::None, tok->row, tok->col}, tok->row, tok->col);
  }
  return ParserResult(ParserResult::ExitCode::okay, std::move(parsedTree),
                      tok->row, tok->col);
}
}  // namespace Jack
