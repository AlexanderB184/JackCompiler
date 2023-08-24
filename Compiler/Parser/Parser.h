#pragma once

#include <vector>

#include "../JackTypes.h"

namespace Jack {

/**
 * Converts a list of Jack::Tokens to an abstract syntax tree of type Jack::ParseTree
 */
ParseTree parse(std::vector<Token>& tokens);

}  // namespace Jack
