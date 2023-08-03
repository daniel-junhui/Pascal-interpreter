// Copyright 2023 Zhu Junhui

#include "interpreter.h"

namespace Pascal {

int Interpreter::interpret() {
  auto tree = this->parser_.parse();
  return tree->accept(this);
}

int Interpreter::visit(const Number* node) {
  return node->value();
}

int Interpreter::visit(const BinaryOp* node) {
  switch (node->op()) {
    case Operator::PLUS:
      return node->left()->accept(this) + node->right()->accept(this);
    case Operator::MINUS:
      return node->left()->accept(this) - node->right()->accept(this);
    case Operator::MULTIPLY:
      return node->left()->accept(this) * node->right()->accept(this);
    case Operator::DIVIDE:
      return node->left()->accept(this) / node->right()->accept(this);
    default:
      throw std::runtime_error("Invalid operator");
  }
}

}  // namespace Pascal
