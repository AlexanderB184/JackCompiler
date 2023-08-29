#include "SymbolTable.h"

namespace Jack {

Symbol::Symbol() {}
Symbol::Symbol(std::string const& name, std::string const& type,
               MemorySegment segment, uint16_t offset)
    : name(name), type(type), segment(segment), offset(offset) {}

SymbolTable::SymbolTable() : table(), nFields(0), nArgs(0), nVars(0) {}
SymbolTable::SymbolTable(const SymbolTable& orig)
    : table(orig.table),
      nFields(orig.nFields),
      nArgs(orig.nArgs),
      nVars(orig.nVars) {}
SymbolTable::SymbolTable(SymbolTable&& orig)
    : table(std::move(orig.table)),
      nFields(std::move(orig.nFields)),
      nArgs(std::move(orig.nArgs)),
      nVars(std::move(orig.nVars)) {}
SymbolTable& SymbolTable::operator=(const SymbolTable& rhs) {
  table = rhs.table;
  nFields = rhs.nFields;
  nArgs = rhs.nArgs;
  nVars = rhs.nVars;
  return *this;
}
SymbolTable& SymbolTable::operator=(SymbolTable&& rhs) {
  table = std::move(rhs.table);
  nFields = std::move(rhs.nFields);
  nArgs = std::move(rhs.nArgs);
  nVars = std::move(rhs.nVars);
  return *this;
}

Symbol& SymbolTable::insert(const Symbol& symbol) {
  return table.insert(std::make_pair(symbol.name, symbol)).first->second;
}
Symbol& SymbolTable::insert(Symbol&& symbol) {
  return table.insert(std::make_pair(symbol.name, std::move(symbol)))
      .first->second;
}
Symbol& SymbolTable::insert(std::string const& name, std::string const& type,
                            Symbol::MemorySegment segment) {
  uint16_t offset;
  switch (segment) {
    case Symbol::MemorySegment::Static:
      offset = nStatics++;
      break;
    case Symbol::MemorySegment::Field:
      offset = nFields++;
      break;
    case Symbol::MemorySegment::Argument:
      offset = nArgs++;
      break;
    case Symbol::MemorySegment::Local:
      offset = nVars++;
      break;
    default:
      break;  // UNREACHABLE
  }
  return table
      .emplace(std::piecewise_construct, std::forward_as_tuple(name),
               std::forward_as_tuple(name, type, segment, offset))
      .first->second;
}

bool SymbolTable::contains(std::string const& symbol) const {
  return table.find(symbol) != table.end();
}

const Symbol& SymbolTable::get(std::string const& symbol) const {
  return table.find(symbol)->second;
}

}  // namespace Jack
