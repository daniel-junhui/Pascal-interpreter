// Copyright 2023 Zhu Junhui

#include "interpreter.h"

namespace Pascal {
void Interpreter::error() {
  throw std::runtime_error("Invalid syntax");
}

void Interpreter::eat(Token::Type type) {
  if (current_token_.type() == type) {
    current_token_ = lexer_.get_next_token();
  } else {
    error();
  }
}

int Interpreter::expr() {
  auto result = factor();

  while (current_token_.type() == Token::Type::PLUS ||
         current_token_.type() == Token::Type::MINUS ||
         current_token_.type() == Token::Type::MULTIPLY ||
         current_token_.type() == Token::Type::DIVIDE) {
    auto token = current_token_;
    if (token.type() == Token::Type::PLUS) {
      eat(Token::Type::PLUS);
      result += factor();
    } else if (token.type() == Token::Type::MINUS) {
      eat(Token::Type::MINUS);
      result -= factor();
    } else if (token.type() == Token::Type::MULTIPLY) {
      eat(Token::Type::MULTIPLY);
      result *= factor();
    } else if (token.type() == Token::Type::DIVIDE) {
      eat(Token::Type::DIVIDE);
      result /= factor();
    }
  }

  return result;
}

int Interpreter::factor() {
  auto token = current_token_;
  eat(Token::Type::INTEGER);
  return std::get<int>(token.value());
}

}  // namespace Pascal