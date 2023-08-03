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

 public:
  int visit(const Pascal::BinaryOperation* binary_op) override {
    print_depth();
    std::cout << "BinaryOperation\n";
    std::cout << "left: \n";
    ++depth_;
    binary_op->left()->accept(this);
    --depth_;
    std::cout << "right: \n";
    ++depth_;
    binary_op->right()->accept(this);
    --depth_;

    return 0;
  }

  int visit(const Pascal::UnaryOperation* unary_op) override {
    print_depth();
    std::cout << "UnaryOperation\n";
    std::cout << "expr: \n";
    ++depth_;
    unary_op->expr()->accept(this);
    --depth_;

    return 0;
  }

  int visit(const Pascal::Number* number) override {
    print_depth();
    std::cout << "Number\n";
    std::cout << "value: " << number->value() << '\n';
    return 0;
  }

  int visit(const Pascal::Compound* compound) override {
    print_depth();
    std::cout << "Compound\n";
    std::cout << "children: \n";
    ++depth_;
    for (const auto& child : compound->children()) {
      child->accept(this);
    }
    --depth_;

    return 0;
  }

  int visit(const Pascal::Assign* assign) override {
    print_depth();

    std::cout << "Assign\n";
    std::cout << "left: \n";
    ++depth_;
    assign->left()->accept(this);
    --depth_;

    std::cout << "right: \n";
    ++depth_;
    assign->right()->accept(this);
    --depth_;

    return 0;
  }

  int visit(const Pascal::Variable* variable) override {
    print_depth();

    std::cout << "Variable\n";
    std::cout << "value: " << variable->value() << '\n';
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
    if (file.eof()) {
      break;
    }
    text += line;
  }

  Pascal::Parser parser(text);
  auto tree = parser.parse();

  ReadVisitor visitor;
  tree->accept(&visitor);

  return 0;
}