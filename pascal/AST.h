// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <vector>
#include "meta.h"

namespace Pascal {

class Visitor;

class AST {
 public:
  virtual ~AST() = default;

  virtual int accept(Visitor* visitor) = 0;
};

class BinaryOperation;
class UnaryOperation;
class Number;
class Compound;
class Assign;
class Variable;

class Visitor {
 public:
  virtual int visit(const BinaryOperation* binary_op) = 0;
  virtual int visit(const UnaryOperation* unary_op) = 0;
  virtual int visit(const Number* number) = 0;
  virtual int visit(const Compound* compound) = 0;
  virtual int visit(const Assign* assign) = 0;
  virtual int visit(const Variable* variable) = 0;
};

#define ACCEPT_VISITOR                    \
  int accept(Visitor* visitor) override { \
    return visitor->visit(this);          \
  }

class BinaryOperation : public AST {
 public:
  enum class Operator {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
  };

 private:
  std::unique_ptr<AST> left_;
  std::unique_ptr<AST> right_;
  Operator op_;

 public:
  explicit BinaryOperation(std::unique_ptr<AST> left,
                           std::unique_ptr<AST> right, Token op_token)
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

  ACCEPT_VISITOR
};

class UnaryOperation : public AST {
 public:
  enum class Operator {
    PLUS,
    MINUS,
  };

 private:
  std::unique_ptr<AST> expr_;
  Operator op_;

 public:
  explicit UnaryOperation(std::unique_ptr<AST> expr, Token op_token)
      : expr_(std::move(expr)) {
    switch (op_token.type()) {
      case Token::Type::PLUS:
        op_ = Operator::PLUS;
        break;
      case Token::Type::MINUS:
        op_ = Operator::MINUS;
        break;
      default:
        throw std::runtime_error("Invalid operator");
    }
  }

  AST* expr() const { return expr_.get(); }

  ACCEPT_VISITOR
};

class Number : public AST {
 private:
  int value_;

 public:
  explicit Number(Token token) {
    assert(token.type() == Token::Type::INTEGER);
    value_ = std::get<int>(token.value());
  }

  int value() const { return value_; }

  ACCEPT_VISITOR
};

class Compound : public AST {
 private:
  std::vector<std::unique_ptr<AST>> children_;

 public:
  void add_child(std::unique_ptr<AST> child) {
    children_.push_back(std::move(child));
  }

  const std::vector<std::unique_ptr<AST>>& children() const {
    return children_;
  }

  ACCEPT_VISITOR
};

class Variable : public AST {
 private:
  std::string value_;

 public:
  explicit Variable(Token token)
      : value_(std::get<std::string>(token.value())) {
    assert(token.type() == Token::Type::ID);
  }

  const std::string& value() const { return value_; }

  ACCEPT_VISITOR
};

class Assign : public AST {
 private:
  std::unique_ptr<Variable> left_;
  std::unique_ptr<AST> right_;

 public:
  Assign(std::unique_ptr<Variable> left, std::unique_ptr<AST> right)
      : left_(std::move(left)), right_(std::move(right)) {}

  Variable* left() const { return left_.get(); }

  AST* right() const { return right_.get(); }

  ACCEPT_VISITOR
};

}  // namespace Pascal
