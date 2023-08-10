#include "symbol_table.h"
#include <spdlog/spdlog.h>

namespace Pascal {

bool SymbolTable::is_defined(const std::string& name) const {
  return symbols_.find(name) != symbols_.end();
}

void SymbolTable::define(const std::string& name, ValueAST::ValueType type) {
  if (is_defined(name)) {
    throw std::runtime_error("variable \"" + name + "\" already defined!");
  }
  symbols_[name] = type;
}

ValueAST::ValueType SymbolTable::get_type(const std::string& name) const {
  if (!is_defined(name)) {
    throw std::runtime_error("variable \"" + name + "\" not defined!");
  }
  return symbols_.at(name);
}

void SymbolTable::print() const {
  for (const auto& [name, type] : symbols_) {
    spdlog::info("{}: {}", name, ValueAST::type_to_string(type));
  }
}

ValueAST::Value SymbolTableBuilder::visit(const BinaryOperation* node) {
  node->left()->accept(this);
  node->right()->accept(this);
  return {};
}

ValueAST::Value SymbolTableBuilder::visit(const UnaryOperation* node) {
  node->expr()->accept(this);
  return {};
}

ValueAST::Value SymbolTableBuilder::visit(const Number* node) {
  return {};
}

ValueAST::Value SymbolTableBuilder::visit(const Variable* node) {
  const auto var_name = node->value();

  if (!symbol_table_.is_defined(var_name)) {
    throw std::runtime_error("variable \"" + var_name + "\" not defined!");
  }

  return {};
}

void SymbolTableBuilder::visit(const Compound* node) {
  for (const auto& child : node->children()) {
    child->accept(this);
  }
}

void SymbolTableBuilder::visit(const Assign* node) {
  const auto left = node->left();
  const auto var_name = left->value();
  if (!symbol_table_.is_defined(var_name)) {
    throw std::runtime_error("variable \"" + var_name + "\" not defined!");
  }
  node->right()->accept(this);
}

void SymbolTableBuilder::visit(const Program* node) {
  node->block()->accept(this);
}

void SymbolTableBuilder::visit(const Block* node) {
  for (const auto& declaration : node->declarations()) {
    declaration->accept(this);
  }
  node->compound_statement()->accept(this);
}

void SymbolTableBuilder::visit(const VariableDeclaration* node) {
  const auto type = node->type()->value();
  for (const auto& var : node->variables()) {
    symbol_table_.define(var->value(), type);
  }
}

ValueAST::ValueType SymbolTableBuilder::visit(const Type* type) {
  return {};
}

void SymbolTableBuilder::visit(const ProcedureDeclaration* node) {
  node->block()->accept(this);
}

void SymbolTableBuilder::print_symbol_table() const {
  symbol_table_.print();
}

}  // namespace Pascal