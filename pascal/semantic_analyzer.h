// Copyright 2023 Zhu Junhui

#pragma once

#include <iostream>
#include <string>
#include <utility>
#include "AST.h"
#include "symbol_table.h"

namespace Pascal {

class SemanticAnalyzer : public Checker {
 private:
  T::SymbolTable symbol_table_;
  void error(const std::string& msg);

  // debug usage
  int depth_ = 0;
  std::ostream& indent() const;

 public:
  void analyze(Program*);

  std::pair<ValueAST*, ValueAST::ValueType> check(BinaryOperation*) override;
  std::pair<ValueAST*, ValueAST::ValueType> check(UnaryOperation*) override;
  std::pair<ValueAST*, ValueAST::ValueType> check(Number*) override;
  std::pair<ValueAST*, ValueAST::ValueType> check(Variable*) override;
  void check(Compound*) override;
  void check(Assign*) override;
  void check(Program*) override;
  void check(Block*) override;
  void check(VariableDeclaration*) override;
  ValueAST::ValueType check(Type*) override;
  void check(ProcedureDeclaration*) override;
};

}  // namespace Pascal
