// Copyright 2023 Zhu Junhui

#include "semantic_analyzer.h"

namespace Pascal {

void SemanticAnalyzer::error(const std::string& msg) {
  throw std::runtime_error(msg);
}

std::ostream& SemanticAnalyzer::indent() const {
  for (int i = 0; i < depth_; ++i) {
    std::cout << "  ";
  }
  return std::cout;
}

void SemanticAnalyzer::analyze(Program* program) {
  if (DEBUG) {
    indent() << "Semantic analysis starts" << std::endl;
  }
  program->accept(this);
}

void SemanticAnalyzer::check(Program* program) {
  if (DEBUG) {
    indent() << "check program" << std::endl;
  }
  symbol_table_.enter_scope(program->name());
  depth_++;
  program->block()->accept(this);
  symbol_table_.exit_scope();
  depth_--;
}

void SemanticAnalyzer::check(Block* block) {
  if (DEBUG) {
    indent() << "check block" << std::endl;
  }

  depth_++;
  for (const auto& declaration : block->var_declarations()) {
    declaration->accept(this);
  }

  depth_--;

  depth_++;
  for (const auto& declaration : block->procedures_declarations()) {
    declaration->accept(this);
  }
  depth_--;

  depth_++;
  block->compound_statement()->accept(this);
  depth_--;
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
  if (DEBUG) {
    indent() << "check variable declaration" << std::endl;
  }

  depth_++;
  const auto type = var_decl->type()->accept(this);

  if (DEBUG) {
    indent() << "type: " << ValueAST::type_to_string(type) << std::endl;
  }
  const auto& var_names = var_decl->variables();

  if (DEBUG) {
    indent() << "variables: ";
  }
  for (const auto& var : var_names) {
    if (DEBUG) {
      std::cout << var->value() << ", ";
    }
    if (!symbol_table_.define(var->value(), type)) {
      error("variable " + var->value() + " has been declared!");
    }
  }
  std::cout << "\n";

  depth_--;
}

void SemanticAnalyzer::check(Compound* compound) {
  if (DEBUG) {
    indent() << "check compound" << std::endl;
  }
  depth_++;
  for (const auto& child : compound->children()) {
    child->accept(this);
  }
  depth_--;
}

void SemanticAnalyzer::check(Assign* assign) {
  if (DEBUG) {
    indent() << "check assign" << std::endl;
  }

  depth_++;

  const auto left_var = assign->left();

  const auto right_expr = assign->right_release();
  const auto [right_typed, right_type] = right_expr->accept(this);

  delete right_expr;

  assign->set_right(right_typed);

  assert(right_typed->type_checked());

  // check whether defined
  if (!symbol_table_.is_defined(left_var->value(), true)) {
    error("variable " + left_var->value() + " has not been declared!");
  }

  // check whether type is equal
  if (symbol_table_.get_type(left_var->value()).value() != right_type) {
    error("type of left expression is not equal to type of right expression!");
  }

  depth_--;
}

std::pair<ValueAST*, ValueAST::ValueType> SemanticAnalyzer::check(
    Variable* variable) {

  if (DEBUG) {
    indent() << "check variable" << std::endl;
  }

  depth_++;
  const auto var_name = variable->value();

  if (DEBUG) {
    indent() << "variable name: " << var_name << std::endl;
  }
  if (!symbol_table_.is_defined(var_name, true)) {
    error("variable " + var_name + " has not been declared!");
  }

  depth_--;
  return variable->wrap_with_type(symbol_table_.get_type(var_name).value());
}

std::pair<ValueAST*, ValueAST::ValueType> SemanticAnalyzer::check(
    BinaryOperation* op) {
  const auto left = op->left_release();
  const auto right = op->right_release();

  const auto [left_typed, left_type] = left->accept(this);
  const auto [right_typed, right_type] = right->accept(this);

  delete left;
  delete right;

  op->set_left(left_typed);
  op->set_right(right_typed);

  assert(left_typed->type_checked());
  assert(right_typed->type_checked());

  if (op->op() != BinaryOperation::Operator::INTEGER_DIV &&
      op->op() != BinaryOperation::Operator::REAL_DIV) {
    if (left_type != right_type) {
      error(
          "type of left expression is not equal to type of right expression!");
    }
  } else if (op->op() == BinaryOperation::Operator::INTEGER_DIV) {
    if (left_type != ValueAST::ValueType::INTEGER ||
        right_type != ValueAST::ValueType::INTEGER) {
      error("type of left expression or right expression is not integer!");
    }
  } else if (op->op() == BinaryOperation::Operator::REAL_DIV) {
    if (left_type != ValueAST::ValueType::REAL ||
        right_type != ValueAST::ValueType::REAL) {
      error("type of left expression or right expression is not real!");
    }
  }

  return op->wrap_with_type(left_type);
}

std::pair<ValueAST*, ValueAST::ValueType> SemanticAnalyzer::check(
    UnaryOperation* op) {
  const auto expr = op->expr_release();
  const auto [expr_typed, expr_type] = expr->accept(this);
  delete expr;

  op->set_expr(expr_typed);

  assert(expr_typed->type_checked());

  return op->wrap_with_type(expr_type);
}

std::pair<ValueAST*, ValueAST::ValueType> SemanticAnalyzer::check(
    Number* number) {

  if (DEBUG) {
    indent() << "check number" << std::endl;
  }
  depth_++;
  const auto type = number->type();
  if (DEBUG) {
    indent() << "type: " << ValueAST::type_to_string(type) << std::endl;
  }
  depth_--;
  return number->wrap_with_type(type);
}

}  // namespace Pascal
