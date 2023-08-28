#pragma once

#include <iostream>

#include "../CompilerOut/PrintTypes.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
namespace Jack {

void handleError(const LexerResult& res) {
  switch (res.exit_code) {
    case LexerResult::ExitCode::okay:
      return;
    case LexerResult::ExitCode::unclosed_comment:
      std::cout << "On line:" << (int)res.exit_line
                << "unclosed multiline comment" << std::endl;
      return;
    case LexerResult::ExitCode::unclosed_quotes:
      std::cout << "On line:" << (int)res.exit_line
                << "unclosed quotes around string constant" << std::endl;
      return;
    case LexerResult::ExitCode::unknown_symbol:
      std::cout << "On line:" << (int)res.exit_line
                << "unknown symbol " + to_string(res.tokens.back())
                << std::endl;
      return;
  }
}
void handleError(const ParserResult& res) {
  switch (res.exit_code) {
    case ParserResult::ExitCode::okay:
      return;
    case ParserResult::ExitCode::unexpected_token:
      std::cout << "On line:" << (int)res.exit_line
                << "unexpected token: " << res.problemToken
                << " expected token: " << res.wantedToken << std::endl;
      return;
    case ParserResult::ExitCode::missing_token:
      std::cout << "On line:" << (int)res.exit_line << "token missing"
                << std::endl;
      return;
  }
}

}  // namespace Jack
