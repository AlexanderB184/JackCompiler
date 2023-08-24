#pragma once

#include <string>
#include <unordered_set>

namespace Jack {

#define _KEYWORDS                                                           \
  {                                                                         \
    "class", "function", "method", "constructor", "static", "field", "var", \
        "return", "while", "if", "else", "let", "do", "int", "char",        \
        "boolean", "void", "true", "false", "null", "this", "skip"          \
  }

#define isKeyword(WORD) (Jack::keywords.find(WORD) != Jack::keywords.end())

#define symbol(c)                                                          \
  (c == '&' || c == '|' || c == '=' || c == '<' || c == '>' || c == '+' || \
   c == '-' || c == '*' || c == '/' || c == '.' || c == ',' || c == '(' || \
   c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '~' || \
   c == ';')

#define whitespace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

#define letter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')

#define number(c) (c >= '0' && c <= '9')

#define linebreak(c) (c == '\n' || c == '\r')

}  // namespace Jack