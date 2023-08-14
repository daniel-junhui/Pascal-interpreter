// Copyright 2023 Zhu Junhui
#pragma once

#include <iostream>
#include "ast.h"
#include "meta.h"

namespace Pascal {

class Printer : public Visitor {
  using Value = ValueAST::Value;

 private:
  int depth_ = 0;
  bool type_checked_ = false;

  std::ostream& pre_print_depth() {
    for (int i = 0; i < depth_; ++i) {
      std::cout << "  ";
    }
    return std::cout;
  }

 public:
  Value visit(const Pascal::BinaryOperation* binary_op) override {
    if (type_checked_)
      assert(binary_op->type_checked());
    if (binary_op->type_checked()) {
      pre_print_depth() << "BinaryOperation: "
                        << ValueAST::type_to_string(binary_op->type()) << '\n';
    } else {
      pre_print_depth() << "BinaryOperation\n";
    }

    ++depth_;
    pre_print_depth() << "Left: \n";
    binary_op->left()->accept(this);
    --depth_;

    ++depth_;
    pre_print_depth() << "Right: \n";
    binary_op->right()->accept(this);
    --depth_;

    return 0;
  }

  Value visit(const Pascal::UnaryOperation* unary_op) override {
    if (type_checked_)
      assert(unary_op->type_checked());
    if (unary_op->type_checked()) {
      pre_print_depth() << "UnaryOperation: "
                        << ValueAST::type_to_string(unary_op->type()) << '\n';
    } else {
      pre_print_depth() << "UnaryOperation\n";
    }

    ++depth_;
    pre_print_depth() << "Expr: \n";
    unary_op->expr()->accept(this);
    --depth_;

    return 0;
  }

  Value visit(const Pascal::Number* number) override {
    if (type_checked_)
      assert(number->type_checked());
    if (number->type_checked()) {
      pre_print_depth() << "Number: "
                        << ValueAST::type_to_string(number->type()) << '\n';
    } else {
      pre_print_depth() << "Number\n";
    }

    if (std::holds_alternative<double>(number->value())) {
      pre_print_depth() << "Value: " << std::get<double>(number->value())
                        << '\n';
      return 0;
    } else {
      pre_print_depth() << "Value: " << std::get<int>(number->value()) << '\n';
      return 0;
    }
  }

  void visit(const Pascal::Compound* compound) override {
    pre_print_depth() << "Compound\n";

    ++depth_;
    pre_print_depth() << "Children:\n";
    for (const auto& child : compound->children()) {
      child->accept(this);
    }
    --depth_;
  }

  void visit(const Pascal::Assign* assign) override {
    pre_print_depth() << "Assign\n";

    ++depth_;
    pre_print_depth() << "Left: \n";
    assign->left()->accept(this);
    --depth_;

    ++depth_;
    pre_print_depth() << "Right: \n";
    assign->right()->accept(this);
    --depth_;
  }

  Value visit(const Pascal::Variable* variable) override {
    pre_print_depth() << "Value: " << variable->value() << '\n';
    return 0;
  }

  Pascal::ValueAST::ValueType visit(const Pascal::Type* type) override {
    pre_print_depth() << "Type: " << type->to_string() << '\n';
    return {};
  }

  void visit(const Pascal::VariableDeclaration* var_decl) override {
    pre_print_depth() << "VariableDeclaration\n";

    ++depth_;
    pre_print_depth() << "Type: \n";
    var_decl->type()->accept(this);
    --depth_;

    ++depth_;
    pre_print_depth() << "Variables: \n";
    for (const auto& var : var_decl->variables()) {
      var->accept(this);
    }
    --depth_;
  }

  void visit(const Pascal::Block* block) override {
    pre_print_depth() << "Block\n";

    ++depth_;
    pre_print_depth() << block->var_declarations().size()
                      << " variable_declarations: \n";

    for (const auto& declaration : block->var_declarations()) {
      declaration->accept(this);
    }
    --depth_;

    ++depth_;

    pre_print_depth() << block->procedures_declarations().size()
                      << " procedure_declarations: \n";

    for (const auto& procedure_decl : block->procedures_declarations()) {
      procedure_decl->accept(this);
    }
    --depth_;

    ++depth_;
    pre_print_depth() << "Compound statement: \n";
    block->compound_statement()->accept(this);
    --depth_;
  }

  void visit(const Pascal::Program* program) override {
    pre_print_depth() << "Program\n";
    pre_print_depth() << "name: " << program->name() << '\n';

    ++depth_;
    pre_print_depth() << "Block: \n";
    program->block()->accept(this);
    --depth_;
  }

  void visit(const Pascal::ProcedureDeclaration* procedure_decl) override {
    pre_print_depth() << "ProcedureDeclaration\n";
    pre_print_depth() << "name: " << procedure_decl->name() << '\n';

    ++depth_;
    pre_print_depth() << "Block: \n";
    procedure_decl->block()->accept(this);
    --depth_;
  }

  void set_type_checked(bool type_checked) { type_checked_ = type_checked; }
};

}  // namespace Pascal
