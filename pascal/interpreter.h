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

  void print_global_scope() const;
};
}  // namespace Pascal
