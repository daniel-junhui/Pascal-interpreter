// Copyright 2023 Zhu Junhui

#include "parser.h"

namespace Pascal {

void Parser::error() {
  throw std::runtime_error("Invalid syntax");
}

std::unique_ptr<AST> Parser::parse() {
  auto result = expr();
  if (current_token_.type() != Token::Type::END_OF_FILE) {
    error();
  }
  return result;
}

void Parser::eat(Token::Type type) {
  if (current_token_.type() == type) {
    current_token_ = lexer_.get_next_token();
  } else {
    error();
  }
}

std::unique_ptr<AST> Parser::factor() {
  auto token = current_token_;
  if (token.type() == Token::Type::INTEGER) {
    eat(Token::Type::INTEGER);
    return std::make_unique<Number>(token);
  } else if (token.type() == Token::Type::LEFT_PAREN) {
    eat(Token::Type::LEFT_PAREN);
    auto result = expr();
    eat(Token::Type::RIGHT_PAREN);
    return result;
  } else {
    error();
  }

  return nullptr;
}

std::unique_ptr<AST> Parser::expr() {
  auto node = term();

  while (current_token_.type() == Token::Type::PLUS ||
         current_token_.type() == Token::Type::MINUS) {
    auto token = current_token_;
    if (token.type() == Token::Type::PLUS) {
      eat(Token::Type::PLUS);
    } else if (token.type() == Token::Type::MINUS) {
      eat(Token::Type::MINUS);
    }
    node = std::make_unique<BinaryOp>(std::move(node), term(), token);
  }

  return node;
}

std::unique_ptr<AST> Parser::term() {
  auto node = factor();

  while (current_token_.type() == Token::Type::MULTIPLY ||
         current_token_.type() == Token::Type::DIVIDE) {
    auto token = current_token_;
    if (token.type() == Token::Type::MULTIPLY) {
      eat(Token::Type::MULTIPLY);
    } else if (token.type() == Token::Type::DIVIDE) {
      eat(Token::Type::DIVIDE);
    }
    node = std::make_unique<BinaryOp>(std::move(node), factor(), token);
  }

  return node;
}

}  // namespace Pascal