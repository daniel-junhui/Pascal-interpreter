// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "meta.h"

namespace Pascal {

class ValueASTVisitor;
class ValueASTChecker;

class ValueAST {
 public:
  virtual ~ValueAST() = default;
  using Value = std::variant<int, double>;
  enum class ValueType { INTEGER, REAL };
  // Type to string
  static std::string type_to_string(ValueType type) {
    switch (type) {
      case ValueType::INTEGER:
        return "INTEGER";
      case ValueType::REAL:
        return "REAL";
      default:
        throw std::runtime_error("Invalid type");
    }
  }
  virtual Value accept(ValueASTVisitor* visitor) const = 0;
  virtual std::pair<ValueAST*, ValueAST::ValueType> accept(
      ValueASTChecker* checker) = 0;
  virtual bool type_checked() const { return false; }

  virtual std::pair<ValueAST*, ValueType> wrap_with_type(ValueType) = 0;

  virtual ValueType type() const {
    throw std::runtime_error("Not a typed node");
  }
};

template <class T>
requires std::is_base_of_v<ValueAST, T> class TypeChecked : public T {
 private:
  ValueAST::ValueType type_;

 public:
  // use T's constructor
  explicit TypeChecked(ValueAST::ValueType type, T&& other)
      : T(std::forward<T>(other)), type_(type) {}

  ValueAST::ValueType type() const override { return type_; }

  bool type_checked() const override { return true; }

  int get_int(ValueASTVisitor* visitor) const {
    if (type_ == ValueAST::ValueType::INTEGER) {
      return std::get<int>(this->accept(visitor));
    } else {
      throw std::runtime_error("Type error");
    }
  }

  double get_real(ValueASTVisitor* visitor) const {
    if (type_ == ValueAST::ValueType::REAL) {
      return std::get<double>(this->accept(visitor));
    } else {
      throw std::runtime_error("Type error");
    }
  }
};

class BinaryOperation;
class UnaryOperation;
class Number;
class Variable;
class Type;

class ValueASTVisitor {
 public:
  virtual ValueAST::Value visit(const BinaryOperation*) = 0;
  virtual ValueAST::Value visit(const UnaryOperation*) = 0;
  virtual ValueAST::Value visit(const Number*) = 0;
  virtual ValueAST::Value visit(const Variable*) = 0;
  virtual ValueAST::ValueType visit(const Type*) = 0;
};

class ValueASTChecker {
 public:
  virtual std::pair<ValueAST*, ValueAST::ValueType> check(BinaryOperation*) = 0;
  virtual std::pair<ValueAST*, ValueAST::ValueType> check(UnaryOperation*) = 0;
  virtual std::pair<ValueAST*, ValueAST::ValueType> check(Number*) = 0;
  virtual std::pair<ValueAST*, ValueAST::ValueType> check(Variable*) = 0;
  virtual ValueAST::ValueType check(Type*) = 0;
};

class Type {
 private:
  ValueAST::ValueType value_;

 public:
  explicit Type(Token token) {
    switch (token.type()) {
      case Token::Type::INTEGER_TYPE:
        value_ = ValueAST::ValueType::INTEGER;
        break;
      case Token::Type::REAL_TYPE:
        value_ = ValueAST::ValueType::REAL;
        break;
      default:
        throw std::runtime_error("Invalid type");
    }
  }

  ValueAST::ValueType accept(ValueASTVisitor* visitor) {
    return visitor->visit(this);
  }

  ValueAST::ValueType accept(ValueASTChecker* checker) {
    return checker->check(this);
  }

  std::string to_string() const {
    switch (value_) {
      case ValueAST::ValueType::INTEGER:
        return "INTEGER";
      case ValueAST::ValueType::REAL:
        return "REAL";
      default:
        throw std::runtime_error("Invalid type");
    }
  }

  ValueAST::ValueType value() const { return value_; }
};

class Variable : public ValueAST {
 private:
  std::string value_;

 public:
  explicit Variable(Token token)
      : value_(std::get<std::string>(token.value())) {
    assert(token.type() == Token::Type::ID);
  }

  explicit Variable(Variable&& other) : value_(std::move(other.value_)) {}

  const std::string& value() const { return value_; }

  ValueAST::Value accept(ValueASTVisitor* visitor) const override {
    return visitor->visit(this);
  }

  std::pair<ValueAST*, ValueAST::ValueType> accept(
      ValueASTChecker* checker) override {
    return checker->check(this);
  }

  std::pair<ValueAST*, ValueType> wrap_with_type(
      ValueAST::ValueType type) override {
    return {new TypeChecked(type, std::move(*this)), type};
  }
};

class Number : public ValueAST {
 private:
  std::variant<int, double> value_;
  ValueAST::ValueType type_;

 public:
  explicit Number(Token token) {
    switch (token.type()) {
      case Token::Type::INTEGER_CONST:
        type_ = ValueAST::ValueType::INTEGER;
        value_ = std::get<int>(token.value());
        break;
      case Token::Type::REAL_CONST:
        type_ = ValueAST::ValueType::REAL;
        value_ = std::get<double>(token.value());
        break;
      default:
        throw std::runtime_error("Invalid token type");
    }
  }

  explicit Number(Number&& number)
      : value_(std::move(number.value_)), type_(number.type_) {}

  std::variant<int, double> value() const { return value_; }

  ValueAST::ValueType type() const { return type_; }

  ValueAST::Value accept(ValueASTVisitor* visitor) const override {
    return visitor->visit(this);
  }

  std::pair<ValueAST*, ValueAST::ValueType> accept(
      ValueASTChecker* checker) override {
    return checker->check(this);
  }

  std::pair<ValueAST*, ValueType> wrap_with_type(
      ValueAST::ValueType type) override {
    return {new TypeChecked(type, std::move(*this)), type};
  }
};

class BinaryOperation : public ValueAST {
 public:
  enum class Operator {
    PLUS,
    MINUS,
    MULTIPLY,
    INTEGER_DIV,
    REAL_DIV,
  };

 private:
  std::unique_ptr<ValueAST> left_;
  std::unique_ptr<ValueAST> right_;
  Operator op_;

 public:
  explicit BinaryOperation(std::unique_ptr<ValueAST> left,
                           std::unique_ptr<ValueAST> right, Token op_token)
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
      case Token::Type::INTEGER_DIV:
        op_ = Operator::INTEGER_DIV;
        break;
      case Token::Type::REAL_DIV:
        op_ = Operator::REAL_DIV;
        break;
      default:
        throw std::runtime_error("Invalid operator");
    }
  }

  explicit BinaryOperation(BinaryOperation&& other)
      : left_(std::move(other.left_)),
        right_(std::move(other.right_)),
        op_(other.op_) {}

  ValueAST* left() const { return left_.get(); }

  ValueAST* right() const { return right_.get(); }

  ValueAST* left_release() { return left_.release(); }

  ValueAST* right_release() { return right_.release(); }

  Operator op() const { return op_; }

  ValueAST::Value accept(ValueASTVisitor* visitor) const override {
    return visitor->visit(this);
  }

  std::pair<ValueAST*, ValueAST::ValueType> accept(
      ValueASTChecker* checker) override {
    return checker->check(this);
  }

  void set_left(ValueAST* left) { left_.reset(left); }

  void set_right(ValueAST* right) { right_.reset(right); }

  std::pair<ValueAST*, ValueType> wrap_with_type(
      ValueAST::ValueType type) override {
    return {new TypeChecked(type, std::move(*this)), type};
  }
};

class UnaryOperation : public ValueAST {
 public:
  enum class Operator {
    PLUS,
    MINUS,
  };

 private:
  std::unique_ptr<ValueAST> expr_;
  Operator op_;

 public:
  explicit UnaryOperation(std::unique_ptr<ValueAST> expr, Token op_token)
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

  explicit UnaryOperation(UnaryOperation&& other)
      : expr_(std::move(other.expr_)), op_(other.op_) {}

  ValueAST* expr() const { return expr_.get(); }

  ValueAST* expr_release() { return expr_.release(); }

  Operator op() const { return op_; }

  ValueAST::Value accept(ValueASTVisitor* visitor) const override {
    return visitor->visit(this);
  }

  std::pair<ValueAST*, ValueAST::ValueType> accept(
      ValueASTChecker* checker) override {
    return checker->check(this);
  }

  void set_expr(ValueAST* expr) { expr_.reset(expr); }

  std::pair<ValueAST*, ValueType> wrap_with_type(
      ValueAST::ValueType type) override {
    return {new TypeChecked(type, std::move(*this)), type};
  }
};

}  // namespace Pascal
