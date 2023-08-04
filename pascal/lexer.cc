// Copyright 2023 Zhu Junhui

#include "lexer.h"

namespace Pascal {

static const std::unordered_map<std::string, Token> RESERVED_KEYWORDS = {
    {"BEGIN", Token(Token::Type::BEGIN)},
    {"END", Token(Token::Type::END)},
};

std::optional<char> Lexer::peek() {
  auto peek_pos = pos_ + 1;
  if (peek_pos == text_.end()) {
    return std::nullopt;
  } else {
    return *peek_pos;
  }
}

void Lexer::error() {
  throw std::runtime_error("Error parsing input");
}

void Lexer::advance() {
  pos_++;
  if (pos_ == text_.end()) {
    current_char_ = std::nullopt;
  } else {
    current_char_ = *pos_;
  }
}

void Lexer::skip_whitespace() {
  while (current_char_.has_value() && std::isspace(*current_char_)) {
    advance();
  }
}

int Lexer::integer() {
  std::string result;
  while (current_char_.has_value() && std::isdigit(*current_char_)) {
    result.push_back(*current_char_);
    advance();
  }
  return std::stoi(result);
}

Token Lexer::id() {
  std::string result;
  while (current_char_.has_value() && std::isalnum(*current_char_)) {
    result.push_back(*current_char_);
    advance();
  }

  if (auto it = RESERVED_KEYWORDS.find(result); it != RESERVED_KEYWORDS.end()) {
    return it->second;
  }

  return Token(Token::Type::ID, result);
}

Token Lexer::get_next_token() {
  while (current_char_.has_value()) {
    if (std::isspace(*current_char_)) {
      skip_whitespace();
      continue;
    }

    if (std::isdigit(*current_char_)) {
      return Token(Token::Type::INTEGER, integer());
    }

    if (*current_char_ == '+') {
      advance();
      return Token(Token::Type::PLUS);
    }

    if (*current_char_ == '-') {
      advance();
      return Token(Token::Type::MINUS);
    }

    if (*current_char_ == '*') {
      advance();
      return Token(Token::Type::MULTIPLY);
    }

    if (*current_char_ == '/') {
      advance();
      return Token(Token::Type::DIVIDE);
    }

    if (*current_char_ == '(') {
      advance();
      return Token(Token::Type::LEFT_PAREN);
    }

    if (*current_char_ == ')') {
      advance();
      return Token(Token::Type::RIGHT_PAREN);
    }

    if (*current_char_ == ';') {
      advance();
      return Token(Token::Type::SEMI);
    }

    if (*current_char_ == ':' && peek().has_value() && *peek() == '=') {
      advance();
      advance();
      return Token(Token::Type::ASSIGN);
    }

    if (*current_char_ == '.') {
      advance();
      return Token(Token::Type::DOT);
    }

    if (std::isalpha(*current_char_)) {
      return id();
    }

    error();
  }

  return Token(Token::Type::END_OF_FILE);
}

}  // namespace Pascal
