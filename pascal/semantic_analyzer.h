// Copyright 2023 Zhu Junhui

#pragma once

#include "AST.h"

namespace Pascal {

class SemanticAnalyzer : public Visitor {
 public:
  void analyze(const Program*);

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
};

}  // namespace Pascal
