#include "Lexer.h"

#include "../CompileError/Error.h"
#include "../JackTypes.h"
#include "LexerTokenClassification.h"

using std::string;
using std::vector;

typedef string::const_iterator iter_t;

namespace Jack {

#define newline \
  col = 0;      \
  line++;

#define next   \
  word += *it; \
  col++;       \
  it++;

#define skip(n) \
  col += n;     \
  it += n;

#define resetState             \
  word.clear();                \
  STATE = lexerState::UNKNOWN; \
  goto GETTYPE;

#define addToken(type)                                   \
  tokens.emplace_back(std::move(word), type, line, col); \
  resetState;

#define addTokenEndOfFile(type) \
  tokens.emplace_back(std::move(word), type, line, col);

static std::unordered_set<string> keywords(_KEYWORDS);

enum class lexerState {
  UNKNOWN,
  WORD,
  NUMBER,
  STRING,
  SYMBOL,
  LINECOMMENT,
  MULTILINECOMMENT,
  EMPTY
};

Token::Token(string&& val, TokenType type, size_t line, size_t col)
    : value(std::move(val)), type(type), row(line), col(col) {
  val.clear();
}

Token::Token(const string& val, TokenType type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}

lexerResult::lexerResult(lexerResult::ExitCode exit_code,
                         std::vector<Token>&& tokens, size_t line, size_t col)
    : exit_code(exit_code),
      tokens(std::move(tokens)),
      exit_line(line),
      exit_col(col) {
  this->tokens.shrink_to_fit();
}

lexerResult tokenize(string const& script) {
  vector<Token> tokens;
  size_t line = 0, col = 0;
  iter_t it = script.cbegin(), tok_start;
  lexerState STATE = lexerState::UNKNOWN;
  char curr_char, next_char;
  std::string word;
GETTYPE:
  while (*it) {
    curr_char = *it;
    next_char = *(it + 1);
    switch (STATE) {
      case lexerState::UNKNOWN:
        if (letter(curr_char)) {
          STATE = lexerState::WORD;
        } else if (number(curr_char)) {
          STATE = lexerState::NUMBER;
        } else if (whitespace(curr_char)) {
          STATE = lexerState::EMPTY;
        } else if (curr_char == '\"') {
          STATE = lexerState::STRING;
          skip(1);
        } else if (curr_char == '/' && next_char == '/') {
          STATE = lexerState::LINECOMMENT;
          skip(2);
        } else if (curr_char == '/' && next_char == '*') {
          STATE = lexerState::MULTILINECOMMENT;
          skip(2);
        } else if (symbol(curr_char)) {
          STATE = lexerState::SYMBOL;
        } else {
          next;
          addTokenEndOfFile(TokenType::Invalid);
          return lexerResult(lexerResult::ExitCode::unkown_symbol,
                             std::move(tokens), line, col);
        }
        tok_start = it;
        break;
      case lexerState::WORD:
        if (!(letter(curr_char) || number(curr_char))) {
          if (isKeyword(word)) {
            addToken(TokenType::Keyword);
          } else {
            addToken(TokenType::Identifier);
          }
        }
        break;
      case lexerState::NUMBER:
        if (!number(curr_char)) {
          addToken(TokenType::Number);
        }
        break;
      case lexerState::SYMBOL:
        addToken(TokenType::Symbol);
        break;
      case lexerState::STRING:
        if (curr_char == '\"') {
          skip(1);
          addToken(TokenType::String);
        } else if (linebreak(curr_char)) {
          addTokenEndOfFile(TokenType::Invalid);
          return lexerResult(lexerResult::ExitCode::unclosed_quotes,
                             std::move(tokens), line, col);
        }
      case lexerState::LINECOMMENT:
        if (linebreak(curr_char)) STATE = lexerState::EMPTY;
        break;
      case lexerState::MULTILINECOMMENT:
        if (curr_char == '*' && next_char == '/') {
          skip(2);
          resetState;
        }
        break;
      case lexerState::EMPTY:
        if (!whitespace(curr_char)) {
          resetState;
        };
        if (linebreak(curr_char)) {
          newline;
        }
        break;
    }  // switch(STATE)
    next;
  }  // while(*it)
  switch (STATE) {
    case lexerState::WORD:
      if (isKeyword(word)) {
        addTokenEndOfFile(TokenType::Keyword);
      } else {
        addTokenEndOfFile(TokenType::Identifier);
      }
      break;
    case lexerState::NUMBER:
      addTokenEndOfFile(TokenType::Number);
      break;
    case lexerState::SYMBOL:
      addTokenEndOfFile(TokenType::Symbol);
      break;
    case lexerState::STRING:
      addTokenEndOfFile(TokenType::Invalid);
      return lexerResult(lexerResult::ExitCode::unclosed_quotes,
                         std::move(tokens), line, col);
    case lexerState::MULTILINECOMMENT:
      addTokenEndOfFile(TokenType::Invalid);
      return lexerResult(lexerResult::ExitCode::unclosed_comment,
                         std::move(tokens), line, col);
    default:
      break;
  }  // switch(STATE)
  return lexerResult(lexerResult::ExitCode::okay, std::move(tokens), line, col);
}  // tokenize

}  // namespace Jack