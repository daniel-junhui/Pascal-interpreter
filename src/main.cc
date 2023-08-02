// Copyright 2023 Zhu Junhui

#include <iostream>
#include "interpreter.h"

int main() {
  while (true) {
    try {
      std::string text;
      std::cout << "calc> ";
      std::getline(std::cin, text);
      if (text.empty()) {
        continue;
      }
      Pascal::Interpreter interpreter(text);
      std::cout << interpreter.interpret() << '\n';
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
  }
  return 0;
}
