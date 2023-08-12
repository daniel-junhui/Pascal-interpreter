// Copyright 2023 Zhu Junhui

#include "symbol_table.h"

namespace Pascal {

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

}  // namespace Pascal
