// Copyright 2023 Zhu Junhui

#include "semantic_analyzer.h"

namespace Pascal {

void SemanticAnalyzer::error(const std::string& msg) {
  throw std::runtime_error(msg);
}

void SemanticAnalyzer::analyze(Program* program) {
  program->accept(this);
}

void SemanticAnalyzer::check(Program* program) {
  symbol_table_.enter_scope(program->name());
  program->block()->accept(this);
  symbol_table_.exit_scope();
}

void SemanticAnalyzer::check(Block* block) {
  for (const auto& declaration : block->var_declarations()) {
    declaration->accept(this);
  }
  for (const auto& declaration : block->procedures_declarations()) {
    declaration->accept(this);
  }
  block->compound_statement()->accept(this);
}

void SemanticAnalyzer::check(ProcedureDeclaration* procedure_decl) {
  symbol_table_.enter_scope(procedure_decl->name());
  procedure_decl->block()->accept(this);
  symbol_table_.exit_scope();
}

ValueAST::ValueType SemanticAnalyzer::check(Type* type) {
  return type->value();
}

void SemanticAnalyzer::check(VariableDeclaration* var_decl) {
  const auto type = var_decl->type()->accept(this);
  const auto& var_names = var_decl->variables();
  for (const auto& var : var_names) {
    if (!symbol_table_.define(var->value(), type)) {
      error("variable " + var->value() + " has been declared!");
    }
  }
}

void SemanticAnalyzer::check(Compound* compound) {
  for (const auto& child : compound->children()) {
    child->accept(this);
  }
}

void SemanticAnalyzer::check(Assign* assign) {
  assign->left()->accept(this);
  assign->right()->accept(this);
}

ValueAST* SemanticAnalyzer::check(Variable* variable) {
  const auto var_name = variable->value();
  if (!symbol_table_.is_defined(var_name, true)) {
    error("variable " + var_name + " has not been declared!");
  }
  return {};
  // return new TypeChecked<Variable>(symbol_table_.get_type(var_name), variable);
}

ValueAST* SemanticAnalyzer::check(BinaryOperation* op) {
  ValueAST::ValueType left_type;
  ValueAST::ValueType right_type;
  // for left
  if (const auto type_checked_left =
          dynamic_cast<TypeChecked<ValueAST>*>(op->left()->accept(this));
      type_checked_left != nullptr) {
    assert(type_checked_left->type_checked());
    op->set_left(type_checked_left);
    left_type = type_checked_left->type();
  } else {
    error("type of left expression is not checked!");
  }

  // for right
  if (const auto type_checked_right =
          dynamic_cast<TypeChecked<ValueAST>*>(op->right()->accept(this));
      type_checked_right != nullptr) {
    assert(type_checked_right->type_checked());
    op->set_right(type_checked_right);
    right_type = type_checked_right->type();
  } else {
    error("type of right expression is not checked!");
  }

  if (left_type != right_type) {
    error("type of left and right expression is not same!");
  }
  return new TypeChecked<BinaryOperation>(left_type, op);
}

ValueAST* SemanticAnalyzer::check(UnaryOperation* op) {
  if (const auto type_checked_expr =
          dynamic_cast<TypeChecked<ValueAST>*>(op->expr()->accept(this));
      type_checked_expr != nullptr) {
    assert(type_checked_expr->type_checked());
    op->set_expr(type_checked_expr);
    return new TypeChecked<UnaryOperation>(type_checked_expr->type(), op);
  } else {
    error("type of expression is not checked!");
  }
}

ValueAST* SemanticAnalyzer::check(Number* number) {
  const auto type = number->type();
  using TypeCheckedNumber = TypeChecked<Number>;
  return new TypeCheckedNumber(type, number);
}

}  // namespace Pascal
