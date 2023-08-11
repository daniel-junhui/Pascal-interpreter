// Copyright 2023 Zhu Junhui

#include "lexer.h"

namespace Pascal {

static const std::unordered_map<std::string, Token> RESERVED_KEYWORDS = {
    {"BEGIN", Token(Token::Type::BEGIN)},
    {"END", Token(Token::Type::END)},
    {"DIV", Token(Token::Type::INTEGER_DIV)},
    {"PROGRAM", Token(Token::Type::PROGRAM)},
    {"VAR", Token(Token::Type::VAR)},
    {"INTEGER", Token(Token::Type::INTEGER_TYPE)},
    {"REAL", Token(Token::Type::REAL_TYPE)},
    {"PROCEDURE", Token(Token::Type::PROCEDURE)},

    // lower cases
    {"begin", Token(Token::Type::BEGIN)},
    {"end", Token(Token::Type::END)},
    {"div", Token(Token::Type::INTEGER_DIV)},
    {"program", Token(Token::Type::PROGRAM)},
    {"var", Token(Token::Type::VAR)},
    {"integer", Token(Token::Type::INTEGER_TYPE)},
    {"real", Token(Token::Type::REAL_TYPE)},
    {"procedure", Token(Token::Type::PROCEDURE)},
};

std::optional<char> Lexer::peek() {
  auto peek_pos = pos_ + 1;
  if (peek_pos == text_.end()) {
    return std::nullopt;
  } else {
    return *peek_pos;
  }
}

void Lexer::skip_comment() {
  while (current_char_.has_value() && *current_char_ != '}') {
    advance();
  }
  advance();
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

Token Lexer::number() {
  std::string result;
  while (current_char_.has_value() && std::isdigit(*current_char_)) {
    result.push_back(*current_char_);
    advance();
  }

  if (current_char_.has_value() && *current_char_ == '.') {
    result.push_back(*current_char_);
    advance();
    while (current_char_.has_value() && std::isdigit(*current_char_)) {
      result.push_back(*current_char_);
      advance();
    }
    return Token(Token::Type::REAL_CONST, std::stod(result));
  } else {
    return Token(Token::Type::INTEGER_CONST, std::stoi(result));
  }
}

Token Lexer::get_next_token() {
  while (current_char_.has_value()) {
    if (std::isspace(*current_char_)) {
      skip_whitespace();
      continue;
    }

    if (*current_char_ == '{') {
      advance();
      skip_comment();
      continue;
    }

    if (std::isdigit(*current_char_)) {
      return number();
    }

    if (*current_char_ == ':' && peek().has_value() && *peek() == '=') {
      advance();
      advance();
      return Token(Token::Type::ASSIGN);
    }

    if (*current_char_ == ':') {
      advance();
      return Token(Token::Type::COLON);
    }

    if (*current_char_ == ',') {
      advance();
      return Token(Token::Type::COMMA);
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
      return Token(Token::Type::REAL_DIV);
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
