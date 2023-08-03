// Copyright 2023 Zhu Junhui

#include "lexer.h"
#include <iostream>

int main() {
  while (true) {
    try {
      std::string text;
      std::cout << "calc> ";
      std::getline(std::cin, text);
      if (text.empty()) {
        continue;
      }
      // initialize lexer and print tokens line by line
      Pascal::Lexer lexer(text);
      while (true) {
        auto token = lexer.get_next_token();
        if (token.type() == Pascal::Token::Type::END_OF_FILE) {
          break;
        }
        std::cout << '\t' << token() << '\n';
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
  }
  return 0;
}
