#pragma once

#include <vector>

#include "../Token.h"
#include "../ParseTree.h"

namespace Jack {

struct ParserResult {
  enum class ExitCode { okay, unexpected_token, missing_token } exit_code;
  ParseTree abstractSyntaxTree;
  Token problemToken;
  Token wantedToken;
  size_t exit_line, exit_col;
  ParserResult(ExitCode, ParseTree&&, const size_t, const size_t);
  ParserResult(ExitCode, const Token&, const Token&, const size_t,
               const size_t);
};

/**
 * Converts a list of Jack::Tokens to an abstract syntax tree of type
 * Jack::ParseTree
 */
ParserResult parse(const std::vector<Token>& tokens);

ParserResult parseClass(ParseTree&, std::vector<Token>::const_iterator&,
                        std::vector<Token>::const_iterator);
ParserResult parseClassVarDec(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseSubroutine(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseParameterList(ParseTree&, std::vector<Token>::const_iterator&,
                                std::vector<Token>::const_iterator);
ParserResult parseSubroutineBody(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
ParserResult parseVarDec(ParseTree&, std::vector<Token>::const_iterator&,
                         std::vector<Token>::const_iterator);
ParserResult parseStatements(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseLetStatement(ParseTree&, std::vector<Token>::const_iterator&,
                               std::vector<Token>::const_iterator);
ParserResult parseDoStatement(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseIfStatement(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseWhileStatement(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
ParserResult parseReturnStatement(ParseTree&,
                                  std::vector<Token>::const_iterator&,
                                  std::vector<Token>::const_iterator);
ParserResult parseExpression(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseTerm(ParseTree&, std::vector<Token>::const_iterator&,
                       std::vector<Token>::const_iterator);
ParserResult parseExpressionList(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
}  // namespace Jack
