#include "symbol_table.h"
#include <spdlog/spdlog.h>

namespace Pascal {

bool Scope::define(const std::string& name, ValueAST::ValueType type) {
  if (symbols_.find(name) != symbols_.end()) {
    return false;
  }
  symbols_[name] = type;
  return true;
}

ValueAST::ValueType Scope::get_type(const std::string& name) const {
  if (symbols_.find(name) == symbols_.end()) {
    if (enclosing_scope_) {
      return enclosing_scope_->get_type(name);
    }
    throw std::runtime_error("Undefined variable " + name);
  }
  return symbols_.at(name);
}

bool Scope::is_defined(const std::string& name) const {
  if (symbols_.find(name) == symbols_.end()) {
    return false;
  }
  return true;
}

const Scope* Scope::enclosing_scope() const {
  if (enclosing_scope_) {
    return enclosing_scope_.get();
  }
  return nullptr;
}

SymbolTable::SymbolTable() : current_scope_(std::make_shared<Scope>()) {}

void SymbolTable::define(const std::string& name, ValueAST::ValueType type) {
  if (!current_scope_->define(name, type)) {
    error("Variable " + name + " already defined");
  }
}

ValueAST::ValueType SymbolTable::get_type(const std::string& name) const {
  return current_scope_->get_type(name);
}

bool SymbolTable::is_defined(const std::string& name) const {
  return current_scope_->is_defined(name);
}

void SymbolTable::error(const std::string& msg) const {
  spdlog::error("SymbolTable error: {}", msg);
  throw std::runtime_error(msg);
}



}  // namespace Pascal