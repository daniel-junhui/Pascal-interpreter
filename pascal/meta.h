// Copyright 2023 Zhu Junhui

#pragma once

#include <cassert>
#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

namespace Pascal {

constexpr bool DEBUG = true;

class Token {
 public:
  enum class Type {
    // variable type
    INTEGER_TYPE,
    REAL_TYPE,

    // operator
    PLUS,
    MINUS,
    MULTIPLY,
    INTEGER_DIV,
    REAL_DIV,

    // reserved keywords
    PROGRAM,
    VAR,
    BEGIN,
    END,

    // other
    END_OF_FILE,
    LEFT_PAREN,
    RIGHT_PAREN,
    DOT,
    ASSIGN,
    SEMI,
    ID,
    COLON,
    COMMA,
    INTEGER_CONST,
    REAL_CONST,

    PROCEDURE,
  };
  // Type to string
  static std::string type_to_string(Type type) {
    switch (type) {
      case Type::INTEGER_TYPE:
        return "INTEGER_TYPE";
      case Type::REAL_TYPE:
        return "REAL_TYPE";
      case Type::PLUS:
        return "PLUS";
      case Type::MINUS:
        return "MINUS";
      case Type::MULTIPLY:
        return "MULTIPLY";
      case Type::INTEGER_DIV:
        return "INTEGER_DIV";
      case Type::REAL_DIV:
        return "REAL_DIV";
      case Type::PROGRAM:
        return "PROGRAM";
      case Type::VAR:
        return "VAR";
      case Type::BEGIN:
        return "BEGIN";
      case Type::END:
        return "END";
      case Type::END_OF_FILE:
        return "END_OF_FILE";
      case Type::LEFT_PAREN:
        return "LEFT_PAREN";
      case Type::RIGHT_PAREN:
        return "RIGHT_PAREN";
      case Type::DOT:
        return "DOT";
      case Type::ASSIGN:
        return "ASSIGN";
      case Type::SEMI:
        return "SEMI";
      case Type::ID:
        return "ID";
      case Type::COLON:
        return "COLON";
      case Type::COMMA:
        return "COMMA";
      case Type::INTEGER_CONST:
        return "INTEGER_CONST";
      case Type::REAL_CONST:
        return "REAL_CONST";
      case Type::PROCEDURE:
        return "PROCEDURE";
    }
    throw std::runtime_error("Unknown token type");
  }

  using ValueType = std::variant<int, std::string, double>;

 private:
  std::optional<ValueType> value_;
  Type type_;

 public:
  explicit Token(Type type) : type_(type), value_(std::nullopt) {}

  explicit Token(Type type, ValueType value) : type_(type), value_(value) {}

  Type type() const { return type_; }

  ValueType value() const { return *value_; }

  // operator to std::string
  std::string to_string() const {
    switch (type_) {
      case Type::PLUS:
        return "Token(PLUS, +)";
      case Type::MINUS:
        return "Token(MINUS, -)";
      case Type::MULTIPLY:
        return "Token(MULTIPLY, *)";
      case Type::INTEGER_DIV:
        return "Token(INTEGER_DIV, //)";
      case Type::REAL_DIV:
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
      case Type::COLON:
        return "Token(COLON, :)";
      case Type::COMMA:
        return "Token(COMMA, ,)";
      case Type::INTEGER_CONST:
        return "Token(INTEGER_CONST, " +
               std::to_string(std::get<int>(*value_)) + ")";
      case Type::REAL_CONST:
        return "Token(REAL_CONST, " +
               std::to_string(std::get<double>(*value_)) + ")";
      case Type::PROGRAM:
        return "Token(PROGRAM)";
      case Type::VAR:
        return "Token(VAR)";
      case Type::INTEGER_TYPE:
        return "Token(INTEGER_TYPE)";
      case Type::REAL_TYPE:
        return "Token(REAL_TYPE)";
      case Type::PROCEDURE:
        return "Token(PROCEDURE)";
    }
    throw std::runtime_error("Unknown token type");
  }
};
}  // namespace Pascal
