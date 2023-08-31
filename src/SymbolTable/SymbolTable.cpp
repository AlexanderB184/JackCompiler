#include "SymbolTable.h"

namespace Jack {

u_int16_t SymbolTable::nStatics = 0;

Symbol::Symbol() {}
Symbol::Symbol(std::string const& name, std::string const& type,
               MemorySegment segment, uint16_t offset)
    : name(name), type(type), segment(segment), offset(offset) {}

SymbolTable::SymbolTable(const std::string& name)
    : owner(name), table(), nFields(0), nArgs(0), nVars(0) {}
SymbolTable::SymbolTable(const SymbolTable& orig)
    : owner(orig.owner),
      table(orig.table),
      nFields(orig.nFields),
      nArgs(orig.nArgs),
      nVars(orig.nVars) {}
SymbolTable::SymbolTable(SymbolTable&& orig)
    : owner(std::move(orig.owner)),
      table(std::move(orig.table)),
      nFields(std::move(orig.nFields)),
      nArgs(std::move(orig.nArgs)),
      nVars(std::move(orig.nVars)) {}
SymbolTable& SymbolTable::operator=(const SymbolTable& rhs) {
  owner = rhs.owner;
  table = rhs.table;
  nFields = rhs.nFields;
  nArgs = rhs.nArgs;
  nVars = rhs.nVars;
  return *this;
}
SymbolTable& SymbolTable::operator=(SymbolTable&& rhs) {
  owner = std::move(rhs.owner);
  table = std::move(rhs.table);
  nFields = std::move(rhs.nFields);
  nArgs = std::move(rhs.nArgs);
  nVars = std::move(rhs.nVars);
  return *this;
}

bool SymbolTable::insert(const Symbol& symbol) {
  if (contains(symbol.name)) return false;
  return table.insert(std::make_pair(symbol.name, symbol)).second;
}
bool SymbolTable::insert(Symbol&& symbol) {
  if (contains(symbol.name)) return false;
  return table.insert(std::make_pair(symbol.name, std::move(symbol))).second;
}
bool SymbolTable::insert(std::string const& name, std::string const& type,
                         Symbol::MemorySegment segment) {
  if (contains(name)) return false;
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
  return table.insert(std::make_pair(name, Symbol(name, type, segment, offset)))
      .second;
}

bool SymbolTable::contains(std::string const& symbol) const {
  return table.find(symbol) != table.end();
}

const Symbol& SymbolTable::get(std::string const& symbol) const {
  return table.find(symbol)->second;
}

std::unordered_map<std::string, Symbol>::const_iterator SymbolTable::cbegin()
    const {
  return table.cbegin();
}

std::unordered_map<std::string, Symbol>::const_iterator SymbolTable::cend()
    const {
  return table.cend();
}

}  // namespace Jack
