// Copyright 2023 Zhu Junhui

#pragma once
#include <functional>
#include <memory>
#include <utility>
#include "token.h"

namespace Calculator {

class Visitor;

class AST {
 public:
  virtual ~AST() = default;

  virtual int accept(Visitor* visitor) = 0;
};

class BinaryOp;
class Number;
class UnaryOp;

class Visitor {
 public:
  virtual int visit(const BinaryOp* binary_op) = 0;
  virtual int visit(const Number* number) = 0;
  virtual int visit(const UnaryOp* unary_op) = 0;
};

enum class BinaryOperator {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
};

enum class UnaryOperator {
  PLUS,
  MINUS,
};

class BinaryOp : public AST {
 private:
  std::unique_ptr<AST> left_;
  std::unique_ptr<AST> right_;
  BinaryOperator op_;

 public:
  explicit BinaryOp(std::unique_ptr<AST> left, std::unique_ptr<AST> right,
                    Token op_token)
      : left_(std::move(left)), right_(std::move(right)) {
    switch (op_token.type()) {
      case Token::Type::PLUS:
        op_ = BinaryOperator::PLUS;
        break;
      case Token::Type::MINUS:
        op_ = BinaryOperator::MINUS;
        break;
      case Token::Type::MULTIPLY:
        op_ = BinaryOperator::MULTIPLY;
        break;
      case Token::Type::DIVIDE:
        op_ = BinaryOperator::DIVIDE;
        break;
      default:
        throw std::runtime_error("Invalid operator");
    }
  }

  AST* left() const { return left_.get(); }

  AST* right() const { return right_.get(); }

  BinaryOperator op() const { return op_; }

  int accept(Visitor* visitor) override { return visitor->visit(this); }
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

  int accept(Visitor* visitor) override { return visitor->visit(this); }
};

class UnaryOp : public AST {
 private:
  std::unique_ptr<AST> expr_;
  UnaryOperator op_;

 public:
  explicit UnaryOp(std::unique_ptr<AST> expr, Token op_token)
      : expr_(std::move(expr)) {
    switch (op_token.type()) {
      case Token::Type::PLUS:
        op_ = UnaryOperator::PLUS;
        break;
      case Token::Type::MINUS:
        op_ = UnaryOperator::MINUS;
        break;
      default:
        throw std::runtime_error("Invalid operator");
    }
  }

  AST* expr() const { return expr_.get(); }

  UnaryOperator op() const { return op_; }

  int accept(Visitor* visitor) override { return visitor->visit(this); }
};

}  // namespace Calculator
