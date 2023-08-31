#include "SymbolTable.h"
namespace Jack {

#define OKAY() \
  return TableBuildResult{TableBuildResult::ExitCode::okay, "", 0, 0};
#define ERROR()                                                         \
  return TableBuildResult{TableBuildResult::ExitCode::duplicate_symbol, \
                          it->getValue(), it->getLine(), it->getCol()};

typedef std::vector<ParseTree>::const_iterator iterator_t;

TableBuildResult buildSymbolTables(std::vector<SymbolTable>& tables,
                                   const ParseTree& tree) {
  const std::vector<ParseTree>& classBody = tree.getChildren();
  iterator_t it = classBody.cbegin() + 1;  // Start on class name;
  const iterator_t end = classBody.cend();
  if (it >= end) {
    ERROR();
  }
  tables.emplace_back(it->getValue());
  SymbolTable& classTable = tables.back();

  it += 2;
  while (it < end && it->getType() == ParseTree::Type::ClassVarDec) {
    TableBuildResult res = classTable.addFromVarDec(*it);
    if (res.exit_code != TableBuildResult::ExitCode::okay) return res;
    it++;
  }
  while (it < end && it->getType() == ParseTree::Type::Subroutine) {
    tables.emplace_back(classTable);
    SymbolTable& functionTable = tables.back();
    TableBuildResult res = functionTable.addFromSubroutine(*it);
    if (res.exit_code != TableBuildResult::ExitCode::okay) return res;
    it++;
  }
  OKAY();
}

TableBuildResult SymbolTable::addFromSubroutine(const ParseTree& root) {
  const std::vector<ParseTree>& subroutine = root.getChildren();
  iterator_t it = subroutine.cbegin();
  iterator_t const end = subroutine.cend();
  if (it->getValue() == "method") {
    if (!insert("this", owner, Symbol::MemorySegment::Argument)) {
      ERROR();
    }
  }
  it += 4;
  if (it >= end) {
    ERROR();
  }
  TableBuildResult res = addFromParamList(*it);
  if (res.exit_code != TableBuildResult::ExitCode::okay) return res;
  it += 3;
  if (it >= end) {
    ERROR();
  }
  res = addFromSubroutineBody(*it);
  if (res.exit_code != TableBuildResult::ExitCode::okay) return res;
  OKAY();
}

TableBuildResult SymbolTable::addFromSubroutineBody(const ParseTree& root) {
  const std::vector<ParseTree>& subroutine = root.getChildren();
  iterator_t it = subroutine.cbegin();
  iterator_t const end = subroutine.cend();
  while (it < end && it->getType() == ParseTree::Type::VarDec) {
    TableBuildResult res = addFromVarDec(*it);
    if (res.exit_code != TableBuildResult::ExitCode::okay) return res;
    it++;
  }
  OKAY();
}

TableBuildResult SymbolTable::addFromVarDec(const ParseTree& root) {
  std::vector<ParseTree>::const_iterator it = root.getChildren().cbegin();
  std::vector<ParseTree>::const_iterator const end = root.getChildren().cend();
  // Get Segment
  Symbol::MemorySegment segment;
  if (it->getValue() == "var") {
    segment = Symbol::MemorySegment::Local;
  } else if (it->getValue() == "field") {
    segment = Symbol::MemorySegment::Field;
  } else if (it->getValue() == "static") {
    segment = Symbol::MemorySegment::Static;
  }
  it++;
  if (it >= end) {
    ERROR()
  }
  // Get Type
  std::string type = it->getValue();
  it++;
  // Add symbols
  while (it < end) {
    if (!insert(it->getValue(), type, segment)) {
      ERROR();
    }
    it += 2;
  }
  OKAY();
}
TableBuildResult SymbolTable::addFromParamList(const ParseTree& root) {
  std::vector<ParseTree>::const_iterator it = root.getChildren().cbegin();
  std::vector<ParseTree>::const_iterator const end = root.getChildren().cend();
  Symbol::MemorySegment segment = Symbol::MemorySegment::Argument;
  std::string type;
  while (it < end) {
    // Get Type
    type = it->getValue();
    it++;
    if (!insert(it->getValue(), type, segment)) {
      ERROR();
    }
    it += 2;
  }
  OKAY();
}
}  // namespace Jack
