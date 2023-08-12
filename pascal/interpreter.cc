// Copyright 2023 Zhu Junhui

#include "interpreter.h"
#include <string>

namespace Pascal {
void Interpreter::error(const std::string& msg) {
  throw std::runtime_error(msg);
}

void Interpreter::visit(const Program* program) {
  program->block()->accept(this);
}

void Interpreter::visit(const Block* block) {
  for (const auto& declaration : block->declarations()) {
    declaration->accept(this);
  }
  block->compound_statement()->accept(this);
}

ValueAST::ValueType Interpreter::visit(const Type* type) {
  return type->value();
}

void Interpreter::visit(const VariableDeclaration* var_decl) {
  const auto type = var_decl->type()->accept(this);
  const auto& vars = var_decl->variables();

  return;
}

ValueAST::Value Interpreter::visit(const Number* number) {
  return number->value();
}

void Interpreter::visit(const Assign* assign) {
  const auto var_name = assign->left()->value();
  const auto var_value = assign->right()->accept(this);
  global_scope_[var_name] = var_value;
}

ValueAST::Value Interpreter::visit(const Variable* variable) {
  if (auto it = global_scope_.find(variable->value());
      it != global_scope_.end()) {
    return it->second;
  } else {
    error("variable " + variable->value() + " does not exist!");
    return -1;
  }
}

template <class F>
ValueAST::Value Interpreter::binaryOperateValueAST(const ValueAST* left,
                                                   const ValueAST* right,
                                                   F&& f) {
  const auto left_value = left->accept(this);
  const auto right_value = right->accept(this);
  if (std::holds_alternative<int>(left_value) &&
      std::holds_alternative<int>(right_value)) {
    return f(std::get<int>(left_value), std::get<int>(right_value));
  } else if (std::holds_alternative<double>(left_value) &&
             std::holds_alternative<double>(right_value)) {
    return f(std::get<double>(left_value), std::get<double>(right_value));
  } else if (std::holds_alternative<int>(left_value) &&
             std::holds_alternative<double>(right_value)) {
    return f(std::get<int>(left_value), std::get<double>(right_value));
  } else if (std::holds_alternative<double>(left_value) &&
             std::holds_alternative<int>(right_value)) {
    return f(std::get<double>(left_value), std::get<int>(right_value));
  } else {
    throw std::runtime_error("Invalid ValueAST::Value");
  }
}

ValueAST::Value Interpreter::visit(const BinaryOperation* node) {
  using BinaryOperator = BinaryOperation::Operator;

  // get runtime value of std::variant
  switch (node->op()) {
    case BinaryOperator::PLUS:
      return binaryOperateValueAST(
          node->left(), node->right(),
          [](auto&& left, auto&& right) { return left + right; });
    case BinaryOperator::MINUS:
      return binaryOperateValueAST(
          node->left(), node->right(),
          [](auto&& left, auto&& right) { return left - right; });
    case BinaryOperator::MULTIPLY:
      return binaryOperateValueAST(
          node->left(), node->right(),
          [](auto&& left, auto&& right) { return left * right; });
    case BinaryOperator::INTEGER_DIV:
      return binaryOperateValueAST(
          node->left(), node->right(), [](auto&& left, auto&& right) {
            return static_cast<int>(left) / static_cast<int>(right);
          });
    case BinaryOperator::REAL_DIV:
      return binaryOperateValueAST(
          node->left(), node->right(),
          [](auto&& left, auto&& right) { return left / right; });
    default:
      throw std::runtime_error("Invalid BinaryOperator");
  }
}

template <class F>
ValueAST::Value Interpreter::unaryOperate(const ValueAST* expr, F&& f) {
  const auto expr_value = expr->accept(this);
  if (std::holds_alternative<int>(expr_value)) {
    return f(std::get<int>(expr_value));
  } else if (std::holds_alternative<double>(expr_value)) {
    return f(std::get<double>(expr_value));
  } else {
    throw std::runtime_error("Invalid ValueAST::Value");
  }
}

ValueAST::Value Interpreter::visit(const UnaryOperation* node) {
  using UnaryOperator = UnaryOperation::Operator;
  switch (node->op()) {
    case UnaryOperator::PLUS:
      return unaryOperate(node->expr(), [](auto&& expr) { return expr; });
    case UnaryOperator::MINUS:
      return unaryOperate(node->expr(), [](auto&& expr) { return -expr; });
    default:
      throw std::runtime_error("Invalid UnaryOperator");
  }
}

void Interpreter::visit(const Compound* node) {
  for (const auto& child : node->children()) {
    child->accept(this);
  }
}

void Interpreter::print_global_scope() const {
  // use spdlog to print global scope
  auto logger = spdlog::stdout_color_mt("Interpreter");
  logger->info("Global scope:");
  for (const auto& [key, value] : global_scope_) {
    if (std::holds_alternative<int>(value)) {
      logger->info("{}: {}", key, std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
      logger->info("{}: {}", key, std::get<double>(value));
    } else {
      throw std::runtime_error("Invalid ValueAST::Value");
    }
  }
}

}  // namespace Pascal
