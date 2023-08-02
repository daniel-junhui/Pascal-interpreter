// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include "token.h"

namespace Pascal {

class AST {
 public:
  virtual ~AST() = default;
};

enum class Operator {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
};

class BinaryOp : public AST {
 private:
  std::unique_ptr<AST> left_;
  std::unique_ptr<AST> right_;
  Operator op_;

 public:
  explicit BinaryOp(std::unique_ptr<AST> left, std::unique_ptr<AST> right,
                    Token op_token)
      : left_(std::move(left)), right_(std::move(right)) {
    switch (op_token.type()) {
      case Token::Type::PLUS:
        op_ = Operator::PLUS;
        break;
      case Token::Type::MINUS:
        op_ = Operator::MINUS;
        break;
      case Token::Type::MULTIPLY:
        op_ = Operator::MULTIPLY;
        break;
      case Token::Type::DIVIDE:
        op_ = Operator::DIVIDE;
        break;
      default:
        throw std::runtime_error("Invalid operator");
    }
  }

  AST* left() const { return left_.get(); }

  AST* right() const { return right_.get(); }

  Operator op() const { return op_; }
};

class Number : public AST {
 private:
  int value_;

 public:
  explicit Number(Token token) {
    if (token.type() != Token::Type::INTEGER) {
      throw std::runtime_error("Invalid token type");
    }
    value_ = std::get<int>(token.value());
  }

  int value() const { return value_; }
};

}  // namespace Pascal
