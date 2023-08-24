#include "../CompileError/Error.h"
#include "Lexer.h"
#include "LexerTokenClassification.h"

namespace Jack {

Token::Token(string&& val, TokenType type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}

Token::Token(const string& val, TokenType type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}

string copy(char const* start, char const* end) {
  string str;
  str.reserve(end - start + 1);
  int i = 0;
  do {
    str.push_back(*start);
    start++;
  } while (start < end);
  return str;
}

#define symbol(c)                                                          \
  (c == '&' || c == '|' || c == '=' || c == '<' || c == '>' || c == '+' || \
   c == '-' || c == '*' || c == '/' || c == '.' || c == ',' || c == '(' || \
   c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '~' || \
   c == ';')

#define whitespace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

#define letter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')

#define number(c) (c >= '0' && c <= '9')

#define linebreak(c) (c == '\n' || c == '\r')

#define newline \
  col = 0;      \
  line++;

#define next \
  col++;     \
  script++;

#define skip(n) \
  col += n;     \
  script += n;

#define addToken(type)                                           \
  STATE = lexerState::UNKNOWN;                                   \
  tokens.emplace_back(copy(tok_start, script), type, line, col); \
  goto GET_TYPE;

#define addTokenWithWord(type, word) tokens.emplace_back(word, type, line, col);

#define resetState             \
  STATE = lexerState::UNKNOWN; \
  goto GET_TYPE;

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

vector<Token> tokenize(char const* script) {
  size_t line = 0;
  size_t col = 0;
  vector<Token> tokens;
  lexerState STATE = lexerState::UNKNOWN;
  char curr_char, next_char;
  char const* tok_start;
  while (*script) {
    curr_char = *script;
    next_char = *(script + 1);
    switch (STATE) {
      case lexerState::UNKNOWN:
      GET_TYPE:
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
          // UNKNOWN SYMBOL
        }
        tok_start = script;
        break;
      case lexerState::WORD:
        if (symbol(curr_char) || whitespace(curr_char)) {
          string word = copy(tok_start, script);
          if (isKeyword(word)) {
            addTokenWithWord(TokenType::Keyword, word);
            resetState;
          } else {
            addTokenWithWord(TokenType::Identifier, word);
            resetState;
          }
        }
        break;
      case lexerState::NUMBER:
        if (!number(curr_char)) {
          addToken(TokenType::Number);
          resetState;
        }
        break;
      case lexerState::SYMBOL:
        addToken(TokenType::Symbol);
        resetState;
        break;
      case lexerState::STRING:
        if (curr_char == '\"') {
          addToken(TokenType::String);
          skip(1);
          resetState;
        }
        if (linebreak(curr_char)) {
          // UNCLOSED QUOTES
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
        }
        if (linebreak(curr_char)) {
          newline;
        }
        break;
    }
    next;
  }
  switch (STATE) {
    case lexerState::UNKNOWN:
      break;
    case lexerState::WORD: {
      string word = copy(tok_start, script);
      if (isKeyword(word)) {
        addTokenWithWord(TokenType::Keyword, word);
      } else {
        addTokenWithWord(TokenType::Identifier, word);
      }
    } break;
    case lexerState::NUMBER:
      addToken(TokenType::Number);
      break;
    case lexerState::SYMBOL:
      addToken(TokenType::Symbol);
      break;
    case lexerState::STRING:
      // UNCLOSED STRING
      break;
    case lexerState::MULTILINECOMMENT:
      // UNCLOSED COMMENT
      break;
    case lexerState::LINECOMMENT:
    case lexerState::EMPTY:
    default:
      break;
  }
  return tokens;
}

}  // namespace Jack