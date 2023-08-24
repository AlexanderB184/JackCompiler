#pragma once

#include <string>
#include <vector>

#include "../JackTypes.h"

namespace Jack {

/**
 *  Return type of Jack::tokenize
 *  Represents the 4 possible exit conditions for tokenize
 *  okay: the function completed without exception
 *  unknown_symbol: the function encountered a symbol it doesn't recognise
 *  unclosed_quotes: a string constant extends over a new line or isn't closed
 *  unclosed_comment: a multiline comment isn't closed
 */
enum class lexerResult {
  okay,
  unknown_symbol,
  unclosed_quotes,
  unclosed_comment
};

/**
 * Reads through the script and generates tokens as defined in the Syntactic
 * Elements document Returns lexerResult with either okay or and error code
 */
lexerResult tokenize(std::vector<Token>& tokens, const std::string& script);

}  // namespace Jack
