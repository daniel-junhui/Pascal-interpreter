// Copyright 2023 Zhu Junhui

#pragma once

#include <string>
#include <unordered_map>
#include "AST.h"

namespace Pascal {

class Interpreter : public Visitor {
 private:
  std::unordered_map<std::string, ValueAST::Value> global_scope_;
  void error(const std::string& msg);

  template <class F>
  ValueAST::Value binaryOperateValueAST(const ValueAST* left,
                                        const ValueAST* right, F&& f);

  template <class F>
  ValueAST::Value unaryOperate(const ValueAST* expr, F&& f);

 public:
  void print_global_scope() const;

  ValueAST::Value visit(const BinaryOperation* binary_op) override;

  ValueAST::Value visit(const UnaryOperation* unary_op) override;

  ValueAST::Value visit(const Number* number) override;

  ValueAST::Value visit(const Variable* variable) override;

  void visit(const Compound* compound) override;

  void visit(const Assign* assign) override;

  void visit(const Program* program) override;

  void visit(const Block* block) override;

  void visit(const VariableDeclaration* var_decl) override;

  ValueAST::ValueType visit(const Type* type) override;
};
}  // namespace Pascal
