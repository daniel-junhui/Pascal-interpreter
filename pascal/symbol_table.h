// Copyright 2023 Zhu Junhui

#pragma once

#include <unordered_map>
#include "AST.h"

namespace Pascal {
class SymbolTable {
 private:
  std::unordered_map<std::string, ValueAST::ValueType> symbols_;

 public:
  void define(const std::string& name, ValueAST::ValueType type);

  ValueAST::ValueType get_type(const std::string& name) const;

  bool is_defined(const std::string& name) const;

  void print() const;
};

class SymbolTableBuilder : public Visitor {
 private:
  SymbolTable symbol_table_;

 public:
  ValueAST::Value visit(const BinaryOperation*) override;
  ValueAST::Value visit(const UnaryOperation*) override;
  ValueAST::Value visit(const Number*) override;
  ValueAST::Value visit(const Variable*) override;
  void visit(const Compound*) override;
  void visit(const Assign*) override;
  void visit(const Program*) override;
  void visit(const Block*) override;
  void visit(const VariableDeclaration*) override;
  ValueAST::ValueType visit(const Type*) override;
  void visit(const ProcedureDeclaration*) override;

  void print_symbol_table() const;
};
}  // namespace Pascal