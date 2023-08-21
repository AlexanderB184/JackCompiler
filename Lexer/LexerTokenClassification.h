#pragma once

#include <string>
#include <unordered_set>

namespace Jack {

using std::string;
using std::unordered_set;

bool isWhiteSpace(char);
bool isNumber(char);
bool isLetter(char);
bool isSymbol(char);
bool isKeyword(const string&);

enum class TokenType classifyToken(char);

}  // namespace Jack