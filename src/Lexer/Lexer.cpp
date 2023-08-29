#include "Lexer.h"

#include "../CompileError/Error.h"
#include "LexerTokenClassification.h"

using std::string;
using std::vector;

typedef string::const_iterator iter_t;

namespace Jack {

// Resets the column and increments the line number
#define newline \
  col = 0;      \
  line++;

// increments the iterator and adds the character to word
#define next   \
  word += *it; \
  col++;       \
  it++;

// skips n chars in the script, doesn't add them to word

#define skip(n) \
  col += n;     \
  it += n;

// Resets the state, clears the current word and jumps to the start of the loop
#define resetState             \
  word.clear();                \
  STATE = lexerState::UNKNOWN; \
  goto GETTYPE;

// Adds a token to 'tokens' and resets the State, jumping to GETTYPE
#define addToken(type)                                   \
  tokens.emplace_back(std::move(word), type, line, col); \
  resetState;

// Adds a token to 'tokens' without reseting the State
#define addTokenEndOfFile(type) \
  tokens.emplace_back(std::move(word), type, line, col);

// Creates the lexerResult struct with the correct fields and returns it, ending
// the function.
#define exit(exit_code) \
  return LexerResult(exit_code, std::move(tokens), line, col);

// Set of Jack Reserved Words
static std::unordered_set<string> keywords(_KEYWORDS);

// Enum for possible states the state machine in tokenize can take
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

LexerResult::LexerResult(LexerResult::ExitCode exit_code,
                         std::vector<Token>&& tokens, size_t line, size_t col)
    : exit_code(exit_code),
      tokens(std::move(tokens)),
      exit_line(line),
      exit_col(col) {
  this->tokens.shrink_to_fit();
}

LexerResult tokenize(string const& script) {
  vector<Token> tokens;
  size_t line = 0, col = 0, tok_line = 0, tok_col = 0;
  iter_t it = script.cbegin(), tok_start;
  lexerState STATE = lexerState::UNKNOWN;
  char curr_char, next_char;
  std::string word;

GETTYPE:
  while (*it) {
  CHECKENDCOND:
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
          goto CHECKENDCOND;
        } else if (curr_char == '\"') {
          STATE = lexerState::STRING;
          skip(1);
          tok_start = it;
          tok_line = line;
          tok_col = col;
          goto CHECKENDCOND;
        } else if (curr_char == '/' && next_char == '/') {
          STATE = lexerState::LINECOMMENT;
          skip(2);
          goto CHECKENDCOND;
        } else if (curr_char == '/' && next_char == '*') {
          STATE = lexerState::MULTILINECOMMENT;
          skip(2);
          goto CHECKENDCOND;
        } else if (symbol(curr_char)) {
          STATE = lexerState::SYMBOL;
        } else {
          next;
          addTokenEndOfFile(Token::Type::Invalid);
          exit(LexerResult::ExitCode::unknown_symbol);
        }
        tok_start = it;
        tok_line = line;
        tok_col = col;

        break;
      case lexerState::WORD:
        if (!(letter(curr_char) || number(curr_char))) {
          if (isKeyword(word)) {
            addToken(Token::Type::Keyword);
          } else {
            addToken(Token::Type::Identifier);
          }
        }
        break;
      case lexerState::NUMBER:
        if (!number(curr_char)) {
          addToken(Token::Type::Number);
        }
        break;
      case lexerState::SYMBOL:
        addToken(Token::Type::Symbol);
        break;
      case lexerState::STRING:
        if (curr_char == '\"') {
          skip(1);
          addToken(Token::Type::String);
        } else if (linebreak(curr_char)) {
          addTokenEndOfFile(Token::Type::Invalid);
          exit(LexerResult::ExitCode::unclosed_quotes);
        }
      case lexerState::LINECOMMENT:
        if (linebreak(curr_char)) {
          resetState;
        }
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
        addTokenEndOfFile(Token::Type::Keyword);
      } else {
        addTokenEndOfFile(Token::Type::Identifier);
      }
      break;
    case lexerState::NUMBER:
      addTokenEndOfFile(Token::Type::Number);
      break;
    case lexerState::SYMBOL:
      addTokenEndOfFile(Token::Type::Symbol);
      break;
    case lexerState::STRING:
      addTokenEndOfFile(Token::Type::Invalid);
      exit(LexerResult::ExitCode::unclosed_quotes);
    case lexerState::MULTILINECOMMENT:
      addTokenEndOfFile(Token::Type::Invalid);
      exit(LexerResult::ExitCode::unclosed_comment);
    default:
      break;
  }                                      // switch(STATE)
  addTokenEndOfFile(Token::Type::None);  // Last token is always null token
  exit(LexerResult::ExitCode::okay);
}  // tokenize

}  // namespace Jack