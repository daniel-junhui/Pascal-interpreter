#include <fstream>
#include <iostream>
#include <string>
#include "interpreter.h"
#include "parser.h"

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

  Pascal::Interpreter interpreter;
  tree->accept(&interpreter);

  interpreter.print_global_scope();

  return 0;
}