// Copyright 2023 Zhu Junhui

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include "meta.h"

namespace Pascal {
class Lexer {
 private:
  const std::string text_;
  decltype(text_.begin()) pos_;
  std::optional<char> current_char_;

 public:
  explicit Lexer(const std::string& text)
      : text_(text), pos_(text_.begin()), current_char_(*pos_) {
    assert(text_.size() > 0);
  }

  explicit Lexer(std::string&& text)
      : text_(std::move(text)), pos_(text_.begin()), current_char_(*pos_) {
    assert(text_.size() > 0);
  }

 private:
  void error();

  void advance();

  void skip_whitespace();

  int integer();

  Token id();

  std::optional<char> peek();

 public:
  Token get_next_token();
};
}  // namespace Pascal
