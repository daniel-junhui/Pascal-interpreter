// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "ValueAST.h"
#include "meta.h"

namespace Pascal {

class NonValueASTVisitor;
class NonValueASTChecker;

class NonValueAST {
 public:
  virtual ~NonValueAST() = default;
  virtual void accept(NonValueASTVisitor* visitor) const = 0;
  virtual void accept(NonValueASTChecker* checker) = 0;
};

class Compound;
class Assign;
class Program;
class Block;
class VariableDeclaration;
class ProcedureDeclaration;

class NonValueASTVisitor {
 public:
  virtual void visit(const Compound*) = 0;
  virtual void visit(const Assign*) = 0;
  virtual void visit(const Program*) = 0;
  virtual void visit(const Block*) = 0;
  virtual void visit(const VariableDeclaration*) = 0;
  virtual void visit(const ProcedureDeclaration*) = 0;
};

class NonValueASTChecker {
 public:
  virtual void check(Compound*) = 0;
  virtual void check(Assign*) = 0;
  virtual void check(Program*) = 0;
  virtual void check(Block*) = 0;
  virtual void check(VariableDeclaration*) = 0;
  virtual void check(ProcedureDeclaration*) = 0;
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

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }

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

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }

  const std::string& name() const { return name_; }

  Block* block() const { return block_.get(); }
};

class VariableDeclaration : public NonValueAST {
 private:
  std::vector<std::unique_ptr<Variable>> variables_;
  std::unique_ptr<Type> type_;

 public:
  explicit VariableDeclaration(std::vector<std::unique_ptr<Variable>> variables,
                               std::unique_ptr<Type> type)
      : variables_(std::move(variables)), type_(std::move(type)) {}

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }

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

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }

  const std::string& name() const { return name_; }

  Block* block() const { return block_.get(); }
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

  ValueAST* right_release() { return right_.release(); }

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }

  void set_right(ValueAST* right) { right_.reset(right); }
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

  void accept(NonValueASTVisitor* visitor) const override {
    visitor->visit(this);
  }

  void accept(NonValueASTChecker* checker) override { checker->check(this); }
};

}  // namespace Pascal
