#include "../CompileError/Error.h"
#include "Lexer.h"
#include "LexerTokenClassification.h"

namespace Jack {

Token::Token(string&& val, TokenType type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}

Token::Token(const string& val, TokenType type, size_t line, size_t col)
    : value(val), type(type), row(line), col(col) {}

string copy(string::const_iterator start, string::const_iterator end) {
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
  it++;

#define skip(n) \
  col += n;     \
  it += n;

#define addToken(type)                                           \
  STATE = lexerState::UNKNOWN;                                   \
  tokens.emplace_back(copy(tok_start, it), type, line, col); \
  goto GETTYPE;

#define addTokenWithWord(type, word)          \
  STATE = lexerState::UNKNOWN;                \
  tokens.emplace_back(word, type, line, col); \
  goto GETTYPE;

#define addTokenEndOfFile(type) \
  STATE = lexerState::UNKNOWN;  \
  tokens.emplace_back(copy(tok_start, it), type, line, col);

#define addTokenEndOfFileWithWord(type, word) \
  STATE = lexerState::UNKNOWN;                \
  tokens.emplace_back(word, type, line, col);

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

vector<Token> tokenize(string const& script) {
  size_t line = 0;
  size_t col = 0;
  string::const_iterator it = script.cbegin();
  vector<Token> tokens;
  lexerState STATE = lexerState::UNKNOWN;
  char curr_char, next_char;
  string::const_iterator tok_start;
  while (*it) {
    curr_char = *it;
    next_char = *(it + 1);
    switch (STATE) {
      case lexerState::UNKNOWN:
      GETTYPE:
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
        tok_start = it;
        break;
      case lexerState::WORD:
        if (symbol(curr_char) || whitespace(curr_char)) {
          string word = copy(tok_start, it);
          if (isKeyword(word)) {
            addTokenWithWord(TokenType::Keyword, word);
          } else {
            addTokenWithWord(TokenType::Identifier, word);
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
          addToken(TokenType::String);
          skip(1);
        }
        if (linebreak(curr_char)) {
          // UNCLOSED QUOTES
        }
      case lexerState::LINECOMMENT:
        if (linebreak(curr_char)) STATE = lexerState::EMPTY;
        break;
      case lexerState::MULTILINECOMMENT:
        if (curr_char == '*' && next_char == '/') {
          STATE = lexerState::UNKNOWN;
          skip(2);
        }
        break;
      case lexerState::EMPTY:
        if (!whitespace(curr_char)) STATE = lexerState::UNKNOWN;
        if (linebreak(curr_char)) {
          newline;
        }
        break;
    }
    if (STATE != lexerState::UNKNOWN) {
      next;
    }
  }
  switch (STATE) {
    case lexerState::UNKNOWN:
      break;
    case lexerState::WORD: {
      string word = copy(tok_start, it);
      if (isKeyword(word)) {
        addTokenEndOfFileWithWord(TokenType::Keyword, word);
      } else {
        addTokenEndOfFileWithWord(TokenType::Identifier, word);
      }
    } break;
    case lexerState::NUMBER:
      addTokenEndOfFile(TokenType::Number);
      break;
    case lexerState::SYMBOL:
      addTokenEndOfFile(TokenType::Symbol);
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