#pragma once

#include <string>
#include <vector>

#include "../Token.h"

namespace Jack {

/**
 * Return type for the Jack::tokenize function.
 */
struct LexerResult {
    /**
     * The possible exit conditions for the Jack::tokenize function.
     */
    enum class ExitCode {
        okay,            /**< The function completed without exception. */
        unknown_symbol,   /**< The function encountered a symbol it doesn't recognize. */
        unclosed_quotes, /**< A string constant extends over a new line or isn't closed. */
        unclosed_comment /**< A multiline comment isn't closed. */
    } exit_code;

    /**
     * Tokens output from the tokenize function.
     * If an error occurs, the last token inserted will have type Jack::TokenType::Invalid.
     */
    std::vector<Token> tokens;

    /**
     * Exit line and column for the compiler error handler.
     */
    size_t exit_line, exit_col;

    /**
     * Constructor.
     */
    LexerResult(ExitCode, std::vector<Token>&&, size_t, size_t);
};

/**
 * Tokenizes the given script according to the rules defined in the Syntactic
 * Elements document.
 *
 * This function reads through the provided script and generates a sequence
 * of tokens based on the language's syntax rules. If any errors occur during
 * tokenization, the function returns a lexerResult object with an appropriate
 * exit code and stores a token of type Jack::TokenType::Invalid. This token
 * indicates the line and column where the error occurred.
 *
 * @param[in] script The input script to be tokenized.
 * @return The result of the tokenization process, encapsulated in a lexerResult object.
 *
 * @note The tokens vector within the lexerResult object will be modified even if an error occurs,
 *       so it's important to check the exit_code within the returned lexerResult value
 *       to determine the success or failure of the tokenization process.
 */
LexerResult tokenize(const std::string& script);
}
