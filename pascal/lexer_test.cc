// Copyright 2023 Zhu Junhui

#include "lexer.h"
#include <fstream>
#include <iostream>
#include <string>
#include "io.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  const auto text = Pascal::read_file(argv[1]);

  Pascal::Lexer lexer(text);
  while (true) {
    auto token = lexer.get_next_token();
    if (token.type() == Pascal::Token::Type::END_OF_FILE) {
      break;
    }
    std::cout << '\t' << token() << '\n';
  }

  return 0;
}
