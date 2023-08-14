// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <string>
#include <utility>
#include "ast.h"
#include "lexer.h"

namespace Calculator {

class Parser {
 private:
  Lexer lexer_;
  Token current_token_;

 public:
  template <typename T>
  requires std::convertible_to<T, std::string> explicit Parser(T&& text)
      : lexer_(std::forward<T>(text)),
        current_token_(lexer_.get_next_token()) {}

  std::unique_ptr<AST> parse();

 private:
  void error();

  void eat(Token::Type type);

  std::unique_ptr<AST> factor();

  std::unique_ptr<AST> term();

  std::unique_ptr<AST> expr();
};
}  // namespace Calculator
