// Copyright 2023 Zhu Junhui

#include "lexer.h"
#include <fstream>
#include <iostream>
#include <string>

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

// int main() {
//   while (true) {
//     try {
//       std::string text;
//       std::cout << "calc> ";
//       std::getline(std::cin, text);
//       if (text.empty()) {
//         continue;
//       }
//       // initialize lexer and print tokens line by line
//       Pascal::Lexer lexer(text);
//       while (true) {
//         auto token = lexer.get_next_token();
//         if (token.type() == Pascal::Token::Type::END_OF_FILE) {
//           break;
//         }
//         std::cout << '\t' << token() << '\n';
//       }
//     } catch (const std::exception& e) {
//       std::cerr << e.what() << '\n';
//     }
//   }
//   return 0;
// }
