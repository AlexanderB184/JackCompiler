#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "../ParseTree/ParseTree.h"

namespace Jack {

struct Symbol {
  std::string name;
  std::string type;
  enum class MemorySegment { Local, Argument, Static, Field } segment;
  uint16_t offset;
  Symbol(const std::string&, const std::string&, MemorySegment, uint16_t);
  Symbol();
};

struct TableBuildResult {
  enum class ExitCode { okay, duplicate_symbol } exit_code;
  std::string duplicate_symbol_name;
  size_t exit_line, exit_col;
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

  bool insert(const Symbol&);
  bool insert(Symbol&&);
  bool insert(std::string const&, std::string const&, Symbol::MemorySegment);

  bool contains(const std::string&) const;

  const Symbol& get(const std::string&) const;

  TableBuildResult addFromClass(const ParseTree&);
  TableBuildResult addFromSubroutine(const ParseTree&);
  TableBuildResult addFromSubroutineBody(const ParseTree&);
  TableBuildResult addFromVarDec(const ParseTree&);
  TableBuildResult addFromParamList(const ParseTree&);

  // methods to get memory segment sizes

  uint16_t getStaticCount();
  uint16_t getFieldCount();
  uint16_t getArgCount();
  uint16_t getVarCount();
};

uint16_t SymbolTable::nStatics = 0;

TableBuildResult buildSymbolTables(std::vector<SymbolTable>&,
                                      const ParseTree&);

}  // namespace Jack
