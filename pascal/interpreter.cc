#include "interpreter.h"

namespace Pascal {
void Interpreter::error(const std::string& msg) {
  throw std::runtime_error(msg);
}

int Interpreter::visit(const Number* number) {
  return number->value();
}

int Interpreter::visit(const Assign* assign) {
  auto var_name = assign->left()->value();
  auto var_value = assign->right()->accept(this);
  global_scope_[var_name] = var_value;
  return var_value;
}

int Interpreter::visit(const Variable* variable) {
  if (auto it = global_scope_.find(variable->value());
      it != global_scope_.end()) {
    return it->second;
  } else {
    error("variable " + variable->value() + " does not exist!");
    return -1;
  }
}

int Interpreter::visit(const BinaryOperation* node) {
  using BinaryOperator = BinaryOperation::Operator;
  switch (node->op()) {
    case BinaryOperator::PLUS:
      return node->left()->accept(this) + node->right()->accept(this);
    case BinaryOperator::MINUS:
      return node->left()->accept(this) - node->right()->accept(this);
    case BinaryOperator::MULTIPLY:
      return node->left()->accept(this) * node->right()->accept(this);
    case BinaryOperator::DIVIDE:
      return node->left()->accept(this) / node->right()->accept(this);
    default:
      throw std::runtime_error("Invalid BinaryOperator");
  }
}

int Interpreter::visit(const UnaryOperation* node) {
  using UnaryOperator = UnaryOperation::Operator;
  switch (node->op()) {
    case UnaryOperator::PLUS:
      return node->expr()->accept(this);
    case UnaryOperator::MINUS:
      return -node->expr()->accept(this);
    default:
      throw std::runtime_error("Invalid UnaryOperator");
  }
}

int Interpreter::visit(const Compound* node) {
  for (const auto& child : node->children()) {
    child->accept(this);
  }
  return 0;
}

}  // namespace Pascal
