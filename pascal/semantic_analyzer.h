// Copyright 2023 Zhu Junhui

#pragma once

#include "AST.h"
#include "symbol_table.h"

namespace Pascal {

class SemanticAnalyzer : public Checker {
 private:
  SymbolTable symbol_table_;
  void error(const std::string& msg);

 public:
  void analyze(Program*);

  ValueAST* check(BinaryOperation*) override;
  ValueAST* check(UnaryOperation*) override;
  ValueAST* check(Number*) override;
  ValueAST* check(Variable*) override;
  void check(Compound*) override;
  void check(Assign*) override;
  void check(Program*) override;
  void check(Block*) override;
  void check(VariableDeclaration*) override;
  ValueAST::ValueType check(Type*) override;
  void check(ProcedureDeclaration*) override;
};

}  // namespace Pascal
