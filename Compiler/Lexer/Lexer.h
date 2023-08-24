#pragma once

#include <string>
#include <vector>

#include "../JackTypes.h"

namespace Jack {

/**
 * The possible exit conditions for the Jack::tokenize function.
 */
enum class lexerResult {
  okay,            /**The function completed without exception.*/
  unknown_symbol,  /**The function encountered a symbol it doesn't recognize.*/
  unclosed_quotes, /**A string constant extends over a new line or isn't closed.
                    */
  unclosed_comment /**A multiline comment isn't closed. */
};

/**
 * Tokenizes the given script according to the rules defined in the Syntactic
 * Elements document.
 *
 * This function reads through the provided script and generates a sequence
 * of tokens based on the language's syntax rules. If any errors occur during
 * tokenization, the function returns an appropriate lexerResult code and stores
 * a token of type Jack::TokenType::Invalid, which indicates the line and
 * column where the error occurred.
 *
 * @param[out] tokens A vector to store the generated tokens.
 * @param[in] script The input script to be tokenized.
 * @return The result of the tokenization process, indicating success or a
 * specific error.
 *
 * @note The tokens vector will be modified even if an error occurs, so it's
 * important to check the returned lexerResult value to determine the success or
 * failure of the tokenization process.
 */
lexerResult tokenize(std::vector<Token>& tokens, const std::string& script);

}  // namespace Jack
