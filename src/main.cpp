#include <chrono>
#include <iostream>

#include "./CompilerOut/PrintTypes.h"
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"

void handleError(const Jack::LexerResult& res) {
  switch (res.exit_code) {
    case Jack::LexerResult::ExitCode::okay:
      return;
    case Jack::LexerResult::ExitCode::unclosed_comment:
      std::cout << "On line:" << (int)res.exit_line
                << "unclosed multiline comment" << std::endl;
      return;
    case Jack::LexerResult::ExitCode::unclosed_quotes:
      std::cout << "On line:" << (int)res.exit_line
                << "unclosed quotes around string constant" << std::endl;
      return;
    case Jack::LexerResult::ExitCode::unknown_symbol:
      std::cout << "On line:" << (int)res.exit_line
                << "unknown symbol " + Jack::to_string(res.tokens.back())
                << std::endl;
      return;
  }
}
void handleError(const Jack::ParserResult& res) {
  switch (res.exit_code) {
    case Jack::ParserResult::ExitCode::okay:
      return;
    case Jack::ParserResult::ExitCode::unexpected_token:
      std::cout << "On line: " << (int)res.problemToken.row
                << " unexpected token: " << res.problemToken
                << " expected token: " << res.wantedToken << std::endl;
      return;
    case Jack::ParserResult::ExitCode::missing_token:
      std::cout << "On line:" << (int)res.problemToken.row << "token missing"
                << std::endl;
      return;
  }
}

int main(int argc, char const* argv[]) {
  // std::string file = load(path);

  Jack::LexerResult r = Jack::tokenize(
      "class TEST {static int x,y,z;field char w, o; method boolean foo(int "
      "bar, String str) {var int i,j,k;return 0;}}");
  if (r.exit_code != Jack::LexerResult::ExitCode::okay) {
    handleError(r);
    return 1;
  }
  std::cout << r.tokens << std::endl;
  Jack::ParserResult r2 = Jack::parse(r.tokens);
  if (r2.exit_code != Jack::ParserResult::ExitCode::okay) {
    handleError(r2);
    return 1;
  }
  std::cout << r2.abstractSyntaxTree << std::endl;
  std::vector<Jack::SymbolTable> tables;
  Jack::TableBuildResult r3 =
      Jack::buildSymbolTables(tables, r2.abstractSyntaxTree);
  if (r3.exit_code != Jack::TableBuildResult::ExitCode::okay) {
    return 1;
  }
  for (auto table : tables) {
    std::cout << table << std::endl;
  }

  return 0;
}
