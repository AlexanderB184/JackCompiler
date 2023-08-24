#pragma once

#include <vector>

#include "../types.h"

namespace Jack {

/**
 * Converts a list of Jack::Tokens to an abstract syntax tree of type Jack::ParseTree
 */
ParseTree parse(std::vector<Token> const& tokens);

}  // namespace Jack
