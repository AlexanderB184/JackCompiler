#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Jack {

struct Symbol {
  std::string name;
  std::string type;
  enum class MemorySegment { Local, Argument, Static, Field } segment;
  uint16_t offset;
  Symbol(const std::string&, const std::string&, MemorySegment, uint16_t);
  Symbol();
};

/**
 * A symbol table class which stores information about all variables in a scope
 * All symbol tables are scoped to a single class and function
 * The class level table is constructed first and then cloned for the function
 * scoped tables static variable counter is shared among all symbol tables
 */
class SymbolTable {
 private:
  std::unordered_map<std::string, Symbol> table;
  static u_int16_t nStatics;
  u_int16_t nFields;
  u_int16_t nArgs;
  u_int16_t nVars;

 public:
  SymbolTable();
  SymbolTable(const SymbolTable&);
  SymbolTable(SymbolTable&&);
  SymbolTable& operator=(const SymbolTable&);
  SymbolTable& operator=(SymbolTable&&);

  Symbol& insert(const Symbol&);
  Symbol& insert(Symbol&&);
  Symbol& insert(std::string const&, std::string const&, Symbol::MemorySegment);
  bool contains(const std::string&) const;
  const Symbol& get(const std::string&) const;
  
};

uint16_t SymbolTable::nStatics = 0;

}  // namespace Jack
