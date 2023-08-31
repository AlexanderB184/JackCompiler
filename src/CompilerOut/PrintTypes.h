#pragma once
#include <iostream>
#include <vector>

#include "../ParseTree/ParseTree.h"
#include "../SymbolTable/SymbolTable.h"
#include "../Token/Token.h"

namespace Jack {

std::string to_string(ParseTree::Type);
std::string to_string(const ParseTree&, int);
std::string to_string(Token::Type);
std::string to_string(const Token&);
std::string to_string(const std::vector<Token>&);
std::string to_string(const SymbolTable&);
std::string to_string(const Symbol&);
std::string to_string(const Symbol::MemorySegment&);
std::ostream& operator<<(std::ostream&, Token::Type);
std::ostream& operator<<(std::ostream&, const Token&);
std::ostream& operator<<(std::ostream&, const std::vector<Token>&);
std::ostream& operator<<(std::ostream&, const ParseTree&);
std::ostream& operator<<(std::ostream&, const ParseTree::Type&);
std::ostream& operator<<(std::ostream&, const SymbolTable&);
std::ostream& operator<<(std::ostream&, const Symbol&);
std::ostream& operator<<(std::ostream&, const Symbol::MemorySegment&);
}  // namespace Jack
