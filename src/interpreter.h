// Copyright 2023 Zhu Junhui

#pragma once

#include <concepts>
#include <optional>
#include <string>
#include "lexer.h"

namespace Pascal {

class Interpreter {

 private:
  Lexer lexer_;
  Token current_token_;

 public:
  // convertable to string
  template <typename T>
  requires std::convertible_to<T, std::string> explicit Interpreter(T&& text)
      : lexer_(std::forward<T>(text)),
        current_token_(lexer_.get_next_token()) {}

  int expr();

 private:
  void error();

  void eat(Token::Type type);

  int factor();

  int term();
};
}  // namespace Pascal
