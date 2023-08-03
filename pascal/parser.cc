// Copyright 2023 Zhu Junhui

#include "parser.h"
#include <utility>

namespace Pascal {

void Parser::error() {
  throw std::runtime_error("Invalid syntax");
}

void Parser::eat(Token::Type type) {
  if (current_token_.type() == type) {
    current_token_ = lexer_.get_next_token();
  } else {
    error();
  }
}

/*
program: compound_statement DOT
compound_statement: BEGIN statement_list END
statement_list: statement | statement SEMI statement_list
statement: compound_statement | assignment_statement | empty
assignment_statement: variable ASSIGN expr
variable: ID
empty:
expr: term ((PLUS | MINUS) term)*
term: factor ((MUL | DIV) factor)*
factor: PLUS factor | MINUS factor | INTEGER | LPAREN expr RPAREN | variable
*/

std::unique_ptr<AST> Parser::parse() {
  auto result = program();
  eat(Token::Type::END_OF_FILE);
  return result;
}

std::unique_ptr<AST> Parser::program() {
  auto node = compound_statement();
  eat(Token::Type::DOT);
  return node;
}

std::unique_ptr<AST> Parser::compound_statement() {
  eat(Token::Type::BEGIN);
  auto nodes = statement_list();
  eat(Token::Type::END);
  auto root = std::make_unique<Compound>();
  for (auto& node : nodes) {
    root->add_child(std::move(node));
  }
  return root;
}

std::vector<std::unique_ptr<AST>> Parser::statement_list() {
  auto node = statement();
  std::vector<std::unique_ptr<AST>> results;
  if (node) {
    results.push_back(std::move(node));
  }
  while (current_token_.type() == Token::Type::SEMI) {
    eat(Token::Type::SEMI);
    auto node = statement();
    if (node) {
      results.push_back(std::move(node));
    }
  }
  return results;
}

std::unique_ptr<AST> Parser::statement() {
  switch (current_token_.type()) {
    case Token::Type::BEGIN:
      return compound_statement();
      break;
    case Token::Type::ID:
      return assignment_statement();
      break;
    default:
      return empty();
      break;
  }
}

std::unique_ptr<AST> Parser::assignment_statement() {
  auto variable = this->variable();
  eat(Token::Type::ASSIGN);
  auto expr = this->expr();
  return std::make_unique<Assign>(std::move(variable), std::move(expr));
}

std::unique_ptr<Variable> Parser::variable() {
  auto node = std::make_unique<Variable>(current_token_);
  eat(Token::Type::ID);
  return node;
}

std::unique_ptr<AST> Parser::empty() {
  return nullptr;
}

std::unique_ptr<AST> Parser::factor() {
  auto token = current_token_;
  auto type = token.type();
  switch (type) {
    case Token::Type::PLUS:
    case Token::Type::MINUS:
      eat(type);
      return std::make_unique<UnaryOperation>(factor(), token);
      break;

    case Token::Type::INTEGER:
      eat(Token::Type::INTEGER);
      return std::make_unique<Number>(token);
      break;

    case Token::Type::LEFT_PAREN: {
      eat(Token::Type::LEFT_PAREN);
      auto result = expr();
      eat(Token::Type::RIGHT_PAREN);
      return result;
    } break;

    case Token::Type::ID:
      return variable();
      break;
    default:
      break;
  }

  error();
  return nullptr;
}

std::unique_ptr<AST> Parser::expr() {
  auto node = term();

  while (current_token_.type() == Token::Type::PLUS ||
         current_token_.type() == Token::Type::MINUS) {
    auto token = current_token_;
    if (token.type() == Token::Type::PLUS) {
      eat(Token::Type::PLUS);
    } else if (token.type() == Token::Type::MINUS) {
      eat(Token::Type::MINUS);
    }
    node = std::make_unique<BinaryOperation>(std::move(node), term(), token);
  }

  return node;
}

std::unique_ptr<AST> Parser::term() {
  auto node = factor();

  while (current_token_.type() == Token::Type::MULTIPLY ||
         current_token_.type() == Token::Type::DIVIDE) {
    auto token = current_token_;
    if (token.type() == Token::Type::MULTIPLY) {
      eat(Token::Type::MULTIPLY);
    } else if (token.type() == Token::Type::DIVIDE) {
      eat(Token::Type::DIVIDE);
    }
    node = std::make_unique<BinaryOperation>(std::move(node), factor(), token);
  }

  return node;
}

}  // namespace Pascal
