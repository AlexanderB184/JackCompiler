#pragma once
#include <iostream>
#include <vector>

#include "../Types/Token.h"
#include "../Types/ParseTree.h"

namespace Jack {

std::string to_string(Jack::ParseTree::Type);
std::string to_string(const Jack::ParseTree&,int);
std::string to_string(Jack::Token::Type);
std::string to_string(const Jack::Token&);
std::string to_string(const std::vector<Jack::Token>&);
std::ostream& operator<<(std::ostream&, Jack::Token::Type);
std::ostream& operator<<(std::ostream&, const Jack::Token&);
std::ostream& operator<<(std::ostream&, const std::vector<Jack::Token>&);
std::ostream& operator<<(std::ostream&, const ParseTree&);
std::ostream& operator<<(std::ostream&, const ParseTree::Type&);

}  // namespace Jack
