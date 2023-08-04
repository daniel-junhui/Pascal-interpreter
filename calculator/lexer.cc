// Copyright 2023 Zhu Junhui

#include "lexer.h"
#include <stdexcept>

namespace Calculator {
void Lexer::error() {
  throw std::runtime_error("Invalid character");
}

void Lexer::advance() {
  this->pos_++;

  if (this->pos_ == this->text_.end()) {
    this->current_char_ = std::nullopt;
  } else {
    this->current_char_ = *this->pos_;
  }
}

void Lexer::skip_whitespace() {
  while (this->current_char_.has_value() &&
         std::isspace(*this->current_char_)) {
    this->advance();
  }
}

int Lexer::integer() {
  std::string result;
  while (this->current_char_.has_value() &&
         std::isdigit(*this->current_char_)) {
    result.push_back(*this->current_char_);
    this->advance();
  }
  return std::stoi(result);
}

Token Lexer::get_next_token() {
  while (this->current_char_.has_value()) {
    if (std::isspace(*this->current_char_)) {
      this->skip_whitespace();
      continue;
    }

    if (std::isdigit(*this->current_char_)) {
      return Token(Token::Type::INTEGER, this->integer());
    }

    if (*this->current_char_ == '+') {
      this->advance();
      return Token(Token::Type::PLUS);
    }

    if (*this->current_char_ == '-') {
      this->advance();
      return Token(Token::Type::MINUS);
    }

    if (*this->current_char_ == '*') {
      this->advance();
      return Token(Token::Type::MULTIPLY);
    }

    if (*this->current_char_ == '/') {
      this->advance();
      return Token(Token::Type::DIVIDE);
    }

    if (*this->current_char_ == '(') {
      this->advance();
      return Token(Token::Type::LEFT_PAREN);
    }

    if (*this->current_char_ == ')') {
      this->advance();
      return Token(Token::Type::RIGHT_PAREN);
    }

    this->error();
  }

  return Token(Token::Type::END_OF_FILE);
}

}  // namespace Calculator
