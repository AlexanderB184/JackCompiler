#pragma once

#include <vector>

#include "../Token/Token.h"
#include "../ParseTree/ParseTree.h"

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


}  // namespace Jack
