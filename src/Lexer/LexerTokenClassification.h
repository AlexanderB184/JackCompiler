#pragma once

#include <string>
#include <unordered_set>

namespace Jack {

static std::unordered_set<std::string> keywords{
    "class", "function", "method", "constructor", "static", "field",
    "var",   "return",   "while",  "if",          "else",   "let",
    "do",    "int",      "char",   "boolean",     "void",   "true",
    "false", "null",     "this",   "skip"};

bool isKeyword(const std::string& WORD) {
  return Jack::keywords.find(WORD) != Jack::keywords.end();
}

bool symbol(char c) {
  return (c == '&' || c == '|' || c == '=' || c == '<' || c == '>' ||
          c == '+' || c == '-' || c == '*' || c == '/' || c == '.' ||
          c == ',' || c == '(' || c == ')' || c == '[' || c == ']' ||
          c == '{' || c == '}' || c == '~' || c == ';');
}

bool whitespace(char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool letter(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

bool number(char c) { return (c >= '0' && c <= '9'); }

bool linebreak(char c) { return (c == '\n' || c == '\r'); }

}  // namespace Jack