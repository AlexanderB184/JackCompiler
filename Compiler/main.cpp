#include <chrono>
#include <iostream>

#include "./CompilerOut/PrintTypes.h"
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"
#include "JackTypes.h"

int main(int argc, char const *argv[]) {
  // std::string file = load(path);
  std::vector<Jack::Token> tokens;
  Jack::lexerResult r = Jack::tokenize(
      tokens,
      "lot/*s[o*/f\"tokens\"1023//elephant*&jackson mississippi\n\t{(92abs)}]");
  switch (r) {
    case Jack::lexerResult::okay:
      std::cout << tokens;
      break;
    default:
      std::cout << "bad";
      // error
      break;
  }
  // Jack::ParseTree t = Jack::parse(tokens);

  return 0;
}
