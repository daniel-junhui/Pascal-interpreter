// Copyright 2023 Zhu Junhui

#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include "io.h"
#include "spdlog/spdlog.h"

class ReadVisitor : public Pascal::Visitor {
  using Value = Pascal::ValueAST::Value;

 private:
  int depth_ = 0;

  void print_depth() {
    for (int i = 0; i < depth_; ++i) {
      std::cout << '\t';
    }
  }

  std::ostream& pre_print_depth() {
    for (int i = 0; i < depth_; ++i) {
      std::cout << '\t';
    }
    return std::cout;
  }

 public:
  Value visit(const Pascal::BinaryOperation* binary_op) override {
    pre_print_depth() << "BinaryOperation\n";
    pre_print_depth() << "left: \n";
    ++depth_;
    binary_op->left()->accept(this);
    --depth_;
    pre_print_depth() << "right: \n";
    ++depth_;
    binary_op->right()->accept(this);
    --depth_;

    return 0;
  }

  Value visit(const Pascal::UnaryOperation* unary_op) override {
    pre_print_depth() << "UnaryOperation\n";
    pre_print_depth() << "expr: \n";
    ++depth_;
    unary_op->expr()->accept(this);
    --depth_;

    return 0;
  }

  Value visit(const Pascal::Number* number) override {
    pre_print_depth() << "Number\n";
    if (std::holds_alternative<double>(number->value())) {
      pre_print_depth() << "value: " << std::get<double>(number->value())
                        << '\n';
      return 0;
    } else {
      pre_print_depth() << "value: " << std::get<int>(number->value()) << '\n';
      return 0;
    }
  }

  void visit(const Pascal::Compound* compound) override {
    pre_print_depth() << "Compound\n";
    pre_print_depth() << "children: \n";
    ++depth_;
    for (const auto& child : compound->children()) {
      child->accept(this);
    }
    --depth_;
  }

  void visit(const Pascal::Assign* assign) override {
    pre_print_depth() << "Assign\n";
    pre_print_depth() << "left: \n";
    ++depth_;
    assign->left()->accept(this);
    --depth_;

    pre_print_depth() << "right: \n";
    ++depth_;
    assign->right()->accept(this);
    --depth_;
  }

  Value visit(const Pascal::Variable* variable) override {
    pre_print_depth() << "Variable\n";
    pre_print_depth() << "value: " << variable->value() << '\n';
    return 0;
  }

  Pascal::ValueAST::ValueType visit(const Pascal::Type* type) override {
    pre_print_depth() << "Type\n";
    pre_print_depth() << "value: " << type->to_string() << '\n';
    return {};
  }

  void visit(const Pascal::VariableDeclaration* var_decl) override {
    pre_print_depth() << "VariableDeclaration\n";
    pre_print_depth() << "type: \n";
    ++depth_;
    var_decl->type()->accept(this);
    --depth_;

    pre_print_depth() << "variables: \n";
    ++depth_;
    for (const auto& var : var_decl->variables()) {
      var->accept(this);
    }
    --depth_;
  }

  void visit(const Pascal::Block* block) override {
    pre_print_depth() << "Block\n";
    pre_print_depth() << "declarations: \n";

    pre_print_depth() << block->var_declarations().size()
                      << " variable_declarations: \n";

    ++depth_;
    for (const auto& declaration : block->var_declarations()) {
      declaration->accept(this);
    }
    --depth_;

    pre_print_depth() << block->procedures_declarations().size()
                      << " procedure_declarations: \n";
    ++depth_;

    for (const auto& procedure_decl : block->procedures_declarations()) {
      procedure_decl->accept(this);
    }
    --depth_;

    pre_print_depth() << "compound_statement: \n";
    ++depth_;
    block->compound_statement()->accept(this);
    --depth_;
  }

  void visit(const Pascal::Program* program) override {
    pre_print_depth() << "Program\n";
    pre_print_depth() << "name: " << program->name() << '\n';
    pre_print_depth() << "block: \n";
    ++depth_;
    program->block()->accept(this);
    --depth_;
  }

  void visit(const Pascal::ProcedureDeclaration* procedure_decl) override {
    pre_print_depth() << "ProcedureDeclaration\n";
    pre_print_depth() << "name: " << procedure_decl->name() << '\n';
    pre_print_depth() << "block: \n";
    ++depth_;
    procedure_decl->block()->accept(this);
    --depth_;
  }
};

// set log level to debug

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  const auto text = Pascal::read_file(argv[1]);

  Pascal::Parser parser(text);
  auto tree = parser.parse();
  spdlog::set_level(spdlog::level::debug);
  ReadVisitor visitor;
  tree->accept(&visitor);

  return 0;
}
