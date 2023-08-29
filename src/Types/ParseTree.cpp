#include "ParseTree.h"

namespace Jack {

ParseTree::ParseTree(const Token& tok)
    : type((ParseTree::Type)((int)tok.type + 14)), children(), tok(tok) {}

ParseTree::ParseTree(ParseTree::Type type) : type(type), children(), tok() {}

ParseTree::ParseTree(const ParseTree& pt)
    : type(pt.type), children(pt.children), tok(pt.tok) {}

ParseTree::ParseTree(ParseTree&& pt)
    : type(std::move(pt.type)),
      children(std::move(pt.children)),
      tok(std::move(pt.tok)) {}

ParseTree& ParseTree::operator=(const ParseTree& pt) {
  type = pt.type;
  children = pt.children;
  tok = pt.tok;
  return *this;
}

ParseTree& ParseTree::operator=(ParseTree&& pt) {
  type = std::move(pt.type);
  children = std::move(pt.children);
  tok = std::move(pt.tok);
  return *this;
}

}  // namespace Jack
