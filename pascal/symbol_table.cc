// Copyright 2023 Zhu Junhui

#include "symbol_table.h"

namespace Pascal {

namespace T {

bool Scope::define(const std::string& name, ValueAST::ValueType type) {
  if (symbols_.find(name) != symbols_.end()) {
    return false;
  }
  symbols_[name] = type;
  return true;
}

Scope::Scope(const std::string& block_name,
             std::shared_ptr<Scope> enclosing_scope)
    : block_name_(block_name), enclosing_scope_(enclosing_scope) {}

std::optional<ValueAST::ValueType> Scope::get_type(
    const std::string& name) const {
  if (auto it = symbols_.find(name); it != symbols_.end()) {
    return it->second;
  }
  return std::nullopt;
}

bool Scope::is_defined(const std::string& name) const {
  if (auto it = symbols_.find(name); it != symbols_.end()) {
    return true;
  }
  return false;
}

std::shared_ptr<Scope> Scope::enclosing_scope() const {
  return enclosing_scope_;
}

SymbolTable::SymbolTable() {
  current_scope_ = nullptr;
}

bool SymbolTable::define(const std::string& name, ValueAST::ValueType type) {
  if (current_scope_ == nullptr) {
    return false;
  }
  return current_scope_->define(name, type);
}

std::optional<ValueAST::ValueType> SymbolTable::get_type(
    const std::string& name) const {
  if (current_scope_ == nullptr) {
    return std::nullopt;
  }
  return current_scope_->get_type(name);
}

bool SymbolTable::is_defined(const std::string& name, bool local) const {
  if (current_scope_ == nullptr) {
    return false;
  }
  if (local) {
    return current_scope_->is_defined(name);
  }
  const Scope* scope = current_scope_.get();
  while (scope != nullptr) {
    if (scope->is_defined(name)) {
      return true;
    }
    scope = scope->enclosing_scope().get();
  }
  return false;
}

void SymbolTable::enter_scope(const std::string& name) {
  current_scope_ = std::make_shared<Scope>(name, current_scope_);
}

void SymbolTable::exit_scope() {
  if (current_scope_ == nullptr) {
    return;
  }

  current_scope_ = current_scope_->enclosing_scope();
}
}  // namespace T

namespace V {

Scope::Scope(const std::string& block_name,
             std::shared_ptr<Scope> enclosing_scope)
    : block_name_(block_name), enclosing_scope_(enclosing_scope) {}

std::shared_ptr<Scope> Scope::enclosing_scope() const {
  return enclosing_scope_;
}

void Scope::define(const std::string& name, int value) {
  integer_map_[name] = value;
}

void Scope::define(const std::string& name, double value) {
  real_map_[name] = value;
}

int Scope::get_integer(const std::string& name) const {
  return integer_map_.at(name);
}

double Scope::get_real(const std::string& name) const {
  return real_map_.at(name);
}

SymbolTable::SymbolTable() {
  current_scope_ = nullptr;
}

void SymbolTable::enter_scope(const std::string& name) {
  current_scope_ = std::make_shared<Scope>(name, current_scope_);
}

void SymbolTable::exit_scope() {
  if (current_scope_ == nullptr) {
    return;
  }

  current_scope_ = current_scope_->enclosing_scope();
}

void SymbolTable::define(const std::string& name, int value) {
  current_scope_->define(name, value);
}

void SymbolTable::define(const std::string& name, double value) {
  current_scope_->define(name, value);
}

int SymbolTable::get_integer(const std::string& name) const {
  const Scope* scope = current_scope_.get();
  while (scope != nullptr) {
    if (scope->integer_map_.find(name) != scope->integer_map_.end()) {
      return scope->integer_map_.at(name);
    }
    scope = scope->enclosing_scope_.get();
  }
  return 0;
}

double SymbolTable::get_real(const std::string& name) const {
  const Scope* scope = current_scope_.get();
  while (scope != nullptr) {
    if (scope->real_map_.find(name) != scope->real_map_.end()) {
      return scope->real_map_.at(name);
    }
    scope = scope->enclosing_scope_.get();
  }
  return 0.0;
}

}  // namespace V

}  // namespace Pascal
