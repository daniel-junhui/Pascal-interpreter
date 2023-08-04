// Copyright 2023 Zhu Junhui

#include <fstream>
#include <iostream>
#include <string>
#include "interpreter.h"
#include "io.h"
#include "parser.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  const auto text = Pascal::read_file(argv[1]);

  Pascal::Parser parser(text);
  auto tree = parser.parse();

  Pascal::Interpreter interpreter;
  tree->accept(&interpreter);

  interpreter.print_global_scope();

  return 0;
}
