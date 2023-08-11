// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "meta.h"

namespace Pascal {

class Visitor;

class NonValueAST {
 public:
  virtual ~NonValueAST() = default;
  virtual void accept(Visitor* visitor) const = 0;
};

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
  virtual Value accept(Visitor* visitor) const = 0;
};

class BinaryOperation;
class UnaryOperation;
class Number;
class Compound;
class Assign;
class Variable;
class Program;
class Block;
class VariableDeclaration;
class Type;
class ProcedureDeclaration;

class Visitor {
 public:
  virtual ValueAST::Value visit(const BinaryOperation*) = 0;
  virtual ValueAST::Value visit(const UnaryOperation*) = 0;
  virtual ValueAST::Value visit(const Number*) = 0;
  virtual ValueAST::Value visit(const Variable*) = 0;
  virtual void visit(const Compound*) = 0;
  virtual void visit(const Assign*) = 0;
  virtual void visit(const Program*) = 0;
  virtual void visit(const Block*) = 0;
  virtual void visit(const VariableDeclaration*) = 0;
  virtual ValueAST::ValueType visit(const Type*) = 0;
  virtual void visit(const ProcedureDeclaration*) = 0;
};

class Block : public NonValueAST {
 private:
  std::vector<std::unique_ptr<VariableDeclaration>> var_declarations_;
  std::vector<std::unique_ptr<ProcedureDeclaration>> procedures_declarations_;
  std::unique_ptr<Compound> compound_statement_;

 public:
  explicit Block(
      std::vector<std::unique_ptr<VariableDeclaration>> var_declarations,
      std::vector<std::unique_ptr<ProcedureDeclaration>> procedure_declarations,
      std::unique_ptr<Compound> compound_statement)
      : var_declarations_(std::move(var_declarations)),
        procedures_declarations_(std::move(procedure_declarations)),
        compound_statement_(std::move(compound_statement)) {}

  void accept(Visitor* visitor) const override { visitor->visit(this); }

  const std::vector<std::unique_ptr<VariableDeclaration>>& var_declarations()
      const {
    return var_declarations_;
  }

  const std::vector<std::unique_ptr<ProcedureDeclaration>>&
  procedures_declarations() const {
    return procedures_declarations_;
  }

  Compound* compound_statement() const { return compound_statement_.get(); }
};

class ProcedureDeclaration : public NonValueAST {
 private:
  std::string name_;
  std::unique_ptr<Block> block_;

 public:
  explicit ProcedureDeclaration(std::string name, std::unique_ptr<Block> block)
      : name_(std::move(name)), block_(std::move(block)) {}

  void accept(Visitor* visitor) const override { visitor->visit(this); }

  const std::string& name() const { return name_; }

  Block* block() const { return block_.get(); }
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

  ValueAST::ValueType accept(Visitor* visitor) { return visitor->visit(this); }

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

  const std::string& value() const { return value_; }

  ValueAST::Value accept(Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

class VariableDeclaration : public NonValueAST {
 private:
  std::vector<std::unique_ptr<Variable>> variables_;
  std::unique_ptr<Type> type_;

 public:
  explicit VariableDeclaration(std::vector<std::unique_ptr<Variable>> variables,
                               std::unique_ptr<Type> type)
      : variables_(std::move(variables)), type_(std::move(type)) {}

  void accept(Visitor* visitor) const override { visitor->visit(this); }

  Type* type() const { return type_.get(); }

  const std::vector<std::unique_ptr<Variable>>& variables() const {
    return variables_;
  }
};

class Program : public NonValueAST {
 private:
  std::string name_;
  std::unique_ptr<Block> block_;

 public:
  explicit Program(std::string name, std::unique_ptr<Block> block)
      : name_(std::move(name)), block_(std::move(block)) {}

  void accept(Visitor* visitor) const override { visitor->visit(this); }

  const std::string& name() const { return name_; }

  Block* block() const { return block_.get(); }
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

  std::variant<int, double> value() const { return value_; }

  ValueAST::ValueType type() const { return type_; }

  ValueAST::Value accept(Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

class Assign : public NonValueAST {
 private:
  std::unique_ptr<Variable> left_;
  std::unique_ptr<ValueAST> right_;

 public:
  Assign(std::unique_ptr<Variable> left, std::unique_ptr<ValueAST> right)
      : left_(std::move(left)), right_(std::move(right)) {}

  Variable* left() const { return left_.get(); }

  ValueAST* right() const { return right_.get(); }

  void accept(Visitor* visitor) const override { visitor->visit(this); }
};

class Compound : public NonValueAST {
 private:
  std::vector<std::unique_ptr<NonValueAST>> children_;

 public:
  void add_child(std::unique_ptr<NonValueAST> child) {
    children_.emplace_back(std::move(child));
  }

  const std::vector<std::unique_ptr<NonValueAST>>& children() const {
    return children_;
  }

  void accept(Visitor* visitor) const override { visitor->visit(this); }
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

  ValueAST* left() const { return left_.get(); }

  ValueAST* right() const { return right_.get(); }

  Operator op() const { return op_; }

  ValueAST::Value accept(Visitor* visitor) const override {
    return visitor->visit(this);
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

  ValueAST* expr() const { return expr_.get(); }

  Operator op() const { return op_; }

  ValueAST::Value accept(Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

}  // namespace Pascal
