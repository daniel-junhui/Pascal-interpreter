#pragma once

#include <optional>
#include <string>
#include <variant>
#include <cassert>
#include <stdexcept>

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
    BEGIN,
    END,
    DOT,
    ASSIGN,
    SEMI,
    ID,
  };
  // Type to string
  static std::string type_to_string(Type type) {
    switch (type) {
      case Type::INTEGER:
        return "INTEGER";
      case Type::PLUS:
        return "PLUS";
      case Type::MINUS:
        return "MINUS";
      case Type::MULTIPLY:
        return "MULTIPLY";
      case Type::DIVIDE:
        return "DIVIDE";
      case Type::END_OF_FILE:
        return "EOF";
      case Type::LEFT_PAREN:
        return "LEFT_PAREN";
      case Type::RIGHT_PAREN:
        return "RIGHT_PAREN";
      case Type::BEGIN:
        return "BEGIN";
      case Type::END:
        return "END";
      case Type::DOT:
        return "DOT";
      case Type::ASSIGN:
        return "ASSIGN";
      case Type::SEMI:
        return "SEMI";
      case Type::ID:
        return "ID";
    }
    throw std::runtime_error("Unknown token type");
  }

  using ValueType = std::variant<int, std::string>;

 private:
  std::optional<ValueType> value_;
  Type type_;

 public:
  explicit Token(Type type) : type_(type), value_(std::nullopt) {}

  explicit Token(Type type, ValueType value) : type_(type), value_(value) {}

  Type type() const { return type_; }

  ValueType value() const { return *value_; }

  // operator to std::string
  std::string operator()() const {
    switch (type_) {
      case Type::INTEGER:
        return "Token(INTEGER, " + std::to_string(std::get<int>(*value_)) + ")";
      case Type::PLUS:
        return "Token(PLUS, +)";
      case Type::MINUS:
        return "Token(MINUS, -)";
      case Type::MULTIPLY:
        return "Token(MULTIPLY, *)";
      case Type::DIVIDE:
        return "Token(DIVIDE, /)";
      case Type::END_OF_FILE:
        return "Token(END_OF_FILE, EOF)";
      case Type::LEFT_PAREN:
        return "Token(LEFT_PAREN, ()";
      case Type::RIGHT_PAREN:
        return "Token(RIGHT_PAREN, ))";
      case Type::BEGIN:
        return "Token(BEGIN)";
      case Type::END:
        return "Token(END)";
      case Type::DOT:
        return "Token(DOT, .)";
      case Type::ASSIGN:
        return "Token(ASSIGN, :=)";
      case Type::SEMI:
        return "Token(SEMI, ;)";
      case Type::ID:
        return "Token(ID, " + std::get<std::string>(*value_) + ")";
    }
    throw std::runtime_error("Unknown token type");
  }
};
}  // namespace Pascal
