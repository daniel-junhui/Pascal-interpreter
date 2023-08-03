// Copyright 2023 Zhu Junhui

#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <utility>
#include "parser.h"

namespace Pascal {

class Interpreter : public Visitor {
 private:
  Parser parser_;

 public:
  // convertable to string
  template <typename T>
  requires std::convertible_to<T, std::string> explicit Interpreter(T&& text)
      : parser_(std::forward<T>(text)) {}

  int interpret();

 private:
  int visit(const BinaryOp* binary_op) override;

  int visit(const Number* number) override;

  int visit(const UnaryOp* unary_op) override;
};
}  // namespace Pascal
