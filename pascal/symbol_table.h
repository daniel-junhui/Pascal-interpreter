// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include "ast.h"

namespace Pascal {

namespace T {
class Scope {
 private:
  std::string block_name_;
  std::unordered_map<std::string, ValueAST::ValueType> symbols_;
  std::shared_ptr<Scope> enclosing_scope_;

 public:
  explicit Scope(const std::string& block_name,
                 std::shared_ptr<Scope> enclosing_scope = nullptr);

  bool define(const std::string& name, ValueAST::ValueType type);

  std::optional<ValueAST::ValueType> get_type(const std::string& name) const;

  bool is_defined(const std::string& name) const;

  std::shared_ptr<Scope> enclosing_scope() const;
};

class SymbolTable {
 private:
  std::shared_ptr<Scope> current_scope_;

 public:
  SymbolTable();

  bool define(const std::string& name, ValueAST::ValueType type);

  std::optional<ValueAST::ValueType> get_type(const std::string& name) const;

  bool is_defined(const std::string& name, bool local) const;

  void enter_scope(const std::string& name);

  void exit_scope();
};
}  // namespace T

namespace V {
class Scope {
 private:
  std::string block_name_;
  std::unordered_map<std::string, int> integer_map_;
  std::unordered_map<std::string, double> real_map_;
  std::shared_ptr<Scope> enclosing_scope_;

 public:
  explicit Scope(const std::string& block_name,
                 std::shared_ptr<Scope> enclosing_scope = nullptr);

  std::shared_ptr<Scope> enclosing_scope() const;

  void define(const std::string& name, int value);

  void define(const std::string& name, double value);

  int get_integer(const std::string& name) const;

  double get_real(const std::string& name) const;
};

class SymbolTable {
 private:
  std::shared_ptr<Scope> current_scope_;

 public:
  SymbolTable();

  void define(const std::string& name, int value);

  void define(const std::string& name, double value);

  int get_integer(const std::string& name) const;

  double get_real(const std::string& name) const;

  void enter_scope(const std::string& name);

  void exit_scope();
};

}  // namespace V

}  // namespace Pascal
