// Copyright 2023 Zhu Junhui

#pragma once

#include <string>
#include <unordered_map>
#include "AST.h"

namespace Pascal {

class Interpreter : public Visitor {
 private:
  std::unordered_map<std::string, int> global_scope_;
  void error(const std::string& msg);

 public:
  // override visit methods
  int visit(const BinaryOperation* binary_op) override;
  int visit(const UnaryOperation* unary_op) override;
  int visit(const Number* number) override;
  int visit(const Compound* compound) override;
  int visit(const Assign* assign) override;
  int visit(const Variable* variable) override;

  void print_global_scope() const;
};
}  // namespace Pascal
