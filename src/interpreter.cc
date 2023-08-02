// Copyright 2023 Zhu Junhui

#include "interpreter.h"

namespace Pascal {

int Interpreter::interpret() {
  auto tree = this->parser_.parse();
  return this->visit(tree.get());
}

template <>
int Interpreter::visit<Number>(Number* node) {
  return node->value();
}

template <>
int Interpreter::visit<BinaryOp>(BinaryOp* node) {
  switch (node->op()) {
    case Operator::PLUS:
      return this->visit(node->left()) + this->visit(node->right());
    case Operator::MINUS:
      return this->visit(node->left()) - this->visit(node->right());
    case Operator::MULTIPLY:
      return this->visit(node->left()) * this->visit(node->right());
    case Operator::DIVIDE:
      return this->visit(node->left()) / this->visit(node->right());
    default:
      throw std::runtime_error("Invalid operator");
  }
}

}  // namespace Pascal
