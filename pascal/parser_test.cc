#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>

class ReadVisitor : public Pascal::Visitor {
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
  int visit(const Pascal::BinaryOperation* binary_op) override {
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

  int visit(const Pascal::UnaryOperation* unary_op) override {
    pre_print_depth() << "UnaryOperation\n";
    pre_print_depth() << "expr: \n";
    ++depth_;
    unary_op->expr()->accept(this);
    --depth_;

    return 0;
  }

  int visit(const Pascal::Number* number) override {
    pre_print_depth() << "Number\n";
    pre_print_depth() << "value: " << number->value() << '\n';
    return 0;
  }

  int visit(const Pascal::Compound* compound) override {
    pre_print_depth() << "Compound\n";
    pre_print_depth() << "children: \n";
    ++depth_;
    for (const auto& child : compound->children()) {
      child->accept(this);
    }
    --depth_;

    return 0;
  }

  int visit(const Pascal::Assign* assign) override {
    pre_print_depth() << "Assign\n";
    pre_print_depth() << "left: \n";
    ++depth_;
    assign->left()->accept(this);
    --depth_;

    pre_print_depth() << "right: \n";
    ++depth_;
    assign->right()->accept(this);
    --depth_;

    return 0;
  }

  int visit(const Pascal::Variable* variable) override {
    pre_print_depth() << "Variable\n";
    pre_print_depth() << "value: " << variable->value() << '\n';
    return 0;
  }
};

int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::fstream file(argv[1]);
  std::string text;

  // read file into text
  while (true) {
    std::string line;
    std::getline(file, line);
    text += line;
    if (file.eof()) {
      break;
    }
  }

  Pascal::Parser parser(text);
  auto tree = parser.parse();

  ReadVisitor visitor;
  tree->accept(&visitor);

  return 0;
}