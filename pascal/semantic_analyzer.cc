// Copyright 2023 Zhu Junhui

#include "semantic_analyzer.h"

namespace Pascal {

void SemanticAnalyzer::error(const std::string& msg) {
  throw std::runtime_error(msg);
}

void SemanticAnalyzer::analyze(const Program* program) {
  program->accept(this);
}

void SemanticAnalyzer::visit(const Program* program) {
  symbol_table_.enter_scope(program->name());
  program->block()->accept(this);
  symbol_table_.exit_scope();
}

void SemanticAnalyzer::visit(const Block* block) {
  for (const auto& declaration : block->var_declarations()) {
    declaration->accept(this);
  }
  for (const auto& declaration : block->procedures_declarations()) {
    declaration->accept(this);
  }
  block->compound_statement()->accept(this);
}

void SemanticAnalyzer::visit(const ProcedureDeclaration* procedure_decl) {
  symbol_table_.enter_scope(procedure_decl->name());
  procedure_decl->block()->accept(this);
  symbol_table_.exit_scope();
}

ValueAST::ValueType SemanticAnalyzer::visit(const Type* type) {
  return type->value();
}

void SemanticAnalyzer::visit(const VariableDeclaration* var_decl) {
  const auto type = var_decl->type()->accept(this);
  const auto& var_names = var_decl->variables();
  for (const auto& var : var_names) {
    if (!symbol_table_.define(var->value(), type)) {
      error("variable " + var->value() + " has been declared!");
    }
  }
}

void SemanticAnalyzer::visit(const Compound* compound) {
  for (const auto& child : compound->children()) {
    child->accept(this);
  }
}

void SemanticAnalyzer::visit(const Assign* assign) {
  assign->left()->accept(this);
  assign->right()->accept(this);
}

ValueAST::Value SemanticAnalyzer::visit(const Variable* variable) {
  const auto var_name = variable->value();
  if (!symbol_table_.is_defined(var_name, true)) {
    error("variable " + var_name + " has not been declared!");
  }
  return {};
}

ValueAST::Value SemanticAnalyzer::visit(const BinaryOperation* op) {
  op->left()->accept(this);
  op->right()->accept(this);
  return {};
}

ValueAST::Value SemanticAnalyzer::visit(const UnaryOperation* op) {
  op->expr()->accept(this);
  return {};
}

ValueAST::Value SemanticAnalyzer::visit(const Number* number) {
  return number->value();
  return {};
}

}  // namespace Pascal
