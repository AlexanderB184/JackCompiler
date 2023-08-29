#include "ParseTree.h"

namespace Jack {

ParseTree::ParseTree(const Token& tok)
    : type((ParseTree::Type)((int)tok.type + 14)), children(), tok(tok) {}

ParseTree::ParseTree(ParseTree::Type type) : type(type), children(), tok() {}

ParseTree::ParseTree(const ParseTree& orig)
    : type(orig.type), children(orig.children), tok(orig.tok) {}

ParseTree::ParseTree(ParseTree&& orig)
    : type(std::move(orig.type)),
      children(std::move(orig.children)),
      tok(std::move(orig.tok)) {}

ParseTree& ParseTree::addChild(ParseTree::Type type) {
  children.emplace_back(type);
  return children.back();
}
ParseTree& ParseTree::addChild(const Token& tok) {
  children.emplace_back(tok);
  return children.back();
}

const std::vector<ParseTree>& ParseTree::getChildren() const {
  return children;
}
const std::string& ParseTree::getValue() const { return tok.value; }
size_t ParseTree::getLine() const { return tok.row; }
size_t ParseTree::getCol() const { return tok.col; }
ParseTree::Type ParseTree::getType() const { return type; }
}  // namespace Jack
