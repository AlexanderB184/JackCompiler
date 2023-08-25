#include <chrono>
#include <iostream>

#include "./CompilerOut/PrintTypes.h"
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"
#include "JackTypes.h"

int main(int argc, char const *argv[]) {
  // std::string file = load(path);

  Jack::lexerResult r = Jack::tokenize("lots[of\"tokens\"1023/elephant*&jackson mississippi\n\t{(92abs)}]");
  std::cout << r.tokens << std::endl;
  // Jack::ParseTree t = Jack::parse(tokens);

  return 0;
}
