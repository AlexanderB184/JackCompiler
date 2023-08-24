#pragma once

#include <stdexcept>
#include <string>
namespace Jack {

struct Token;

void printline(int);

class CompileError : public std::exception {};

class InvalidSymbol : public CompileError {
 public:
  const char* what();
};
class UnclosedQuotes : public CompileError {
 public:
  const char* what();
};
class UnclosedComment : public CompileError {
 public:
  const char* what();
};

void throwError(std::string, Token);

}  // namespace Jack
