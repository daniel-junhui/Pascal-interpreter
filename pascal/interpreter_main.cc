// Copyright 2023 Zhu Junhui

#include <fstream>
#include <iostream>
#include <string>
// #include "interpreter.h"
#include "ast_printer.h"
#include "io.h"
#include "meta.h"
#include "parser.h"
#include "semantic_analyzer.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  const auto text = Pascal::read_file(argv[1]);

  Pascal::Parser parser(text);
  auto tree = parser.parse();

  if constexpr (Pascal::DEBUG) {
    auto printer = Pascal::Printer();
    std::cout << "Before semantic analysis:\n";
    tree->accept(&printer);
  }

  Pascal::SemanticAnalyzer analyzer;
  analyzer.analyze(tree.get());

  if constexpr (Pascal::DEBUG) {
    auto printer = Pascal::Printer();
    printer.set_type_checked(true);
    std::cout << "Before semantic analysis:\n";
    tree->accept(&printer);
  }

  return 0;
}
