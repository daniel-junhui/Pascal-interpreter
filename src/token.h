// Copyright 2023 Zhu Junhui

#pragma once

#include <format>
#include <optional>
#include <string>
#include <variant>

namespace Pascal {
class Token {
 public:
  enum class Type {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END_OF_FILE,
    LEFT_PAREN,
    RIGHT_PAREN,
  };

  // Value type is a variant of int, char
  using ValueType = std::variant<int, char>;

 private:
  std::optional<ValueType> value_;
  Type type_;

 public:
  // constructor
  Token(Type type, ValueType value) : type_(type), value_(value) {}

  explicit Token(Type type) : type_(type), value_(std::nullopt) {}

  // string operator
  operator std::string() {
    // achive the same effect as std::format
    std::string type_str;
    switch (type_) {
      case Type::INTEGER:
        type_str = "INTEGER";
        break;
      case Type::PLUS:
        type_str = "PLUS";
        break;
      case Type::MINUS:
        type_str = "MINUS";
        break;
      case Type::END_OF_FILE:
        type_str = "EOF";
        break;
      case Type::MULTIPLY:
        type_str = "MULTIPLY";
        break;
      case Type::DIVIDE:
        type_str = "DIVIDE";
        break;
      case Type::LEFT_PAREN:
        type_str = "LEFT_PAREN";
        break;
      case Type::RIGHT_PAREN:
        type_str = "RIGHT_PAREN";
        break;
      default:
        break;
    }

    if (value_.has_value()) {
      return "Token(" + type_str + ", " +
             std::to_string(std::get<int>(*value_)) + ")";
    } else {
      return "Token(" + type_str + ")";
    }
  }

  std::string operator()() { return std::string(*this); }

  Type type() { return type_; }

  ValueType value() { return *value_; }
};
}  // namespace Pascal
