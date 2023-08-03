// Copyright 2023 Zhu Junhui

#include "interpreter.h"

namespace Calculator {

int Interpreter::interpret() {
  auto tree = this->parser_.parse();
  return tree->accept(this);
}

int Interpreter::visit(const Number* node) {
  return node->value();
}

int Interpreter::visit(const BinaryOp* node) {
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

int Interpreter::visit(const UnaryOp* node) {
  switch (node->op()) {
    case UnaryOperator::PLUS:
      return node->expr()->accept(this);
    case UnaryOperator::MINUS:
      return -node->expr()->accept(this);
    default:
      throw std::runtime_error("Invalid UnaryOperator");
  }
}

}  // namespace Calculator
