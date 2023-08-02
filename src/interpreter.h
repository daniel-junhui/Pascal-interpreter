// Copyright 2023 Zhu Junhui

#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <utility>
#include "parser.h"

namespace Pascal {

class Interpreter {
 private:
  Parser parser_;

 public:
  // convertable to string
  template <typename T>
  requires std::convertible_to<T, std::string> explicit Interpreter(T&& text)
      : parser_(std::forward<T>(text)) {}

  int interpret();

 private:
  template <typename Tree>
  requires std::derived_from<Tree, AST> int visit(Tree* node) {
    // try Tree can be converted to Number, using dynamic_cast
    if (auto* number = dynamic_cast<Number*>(node); number != nullptr) {
      return this->visit(number);
    }
    // try Tree can be converted to BinaryOp, using dynamic_cast
    if (auto* binary_op = dynamic_cast<BinaryOp*>(node); binary_op != nullptr) {
      return this->visit(binary_op);
    }
    throw std::runtime_error("Invalid AST");
  }
};
}  // namespace Pascal
