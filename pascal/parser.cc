// Copyright 2023 Zhu Junhui

#include "parser.h"
#include <utility>

namespace Pascal {

void Parser::error() {
  throw std::runtime_error("Invalid syntax in parser");
}

void Parser::eat(Token::Type type) {
  if (current_token_.type() == type) {
    current_token_ = lexer_.get_next_token();
  } else {
    error();
  }
}

std::unique_ptr<Variable> Parser::variable() {
  auto node = std::make_unique<Variable>(current_token_);
  eat(Token::Type::ID);
  return node;
}

std::unique_ptr<Program> Parser::parse() {
  auto result = program();
  eat(Token::Type::END_OF_FILE);
  return result;
}

std::unique_ptr<Program> Parser::program() {
  eat(Token::Type::PROGRAM);
  auto var_node = variable();
  auto prog_name = var_node->value();
  eat(Token::Type::SEMI);
  auto block_node = block();
  eat(Token::Type::DOT);
  return std::make_unique<Program>(std::move(prog_name), std::move(block_node));
}

std::unique_ptr<Block> Parser::block() {
  auto declarations = this->declarations();
  auto compound_statement = this->compound_statement();
  return std::make_unique<Block>(std::move(declarations),
                                 std::move(compound_statement));
}

std::vector<std::unique_ptr<VariableDeclaration>> Parser::declarations() {
  std::vector<std::unique_ptr<VariableDeclaration>> declarations;
  if (current_token_.type() == Token::Type::VAR) {
    eat(Token::Type::VAR);

    // here is a tricky part, we need to handle multiple variables and they can be zero or more.
    while (current_token_.type() == Token::Type::ID) {
      auto var_decl = variable_declaration();
      declarations.push_back(std::move(var_decl));
      eat(Token::Type::SEMI);
    }
  }
  return declarations;
}

std::unique_ptr<VariableDeclaration> Parser::variable_declaration() {

  auto var_nodes = std::vector<std::unique_ptr<Variable>>();

  // There may be multiple variables declared in one declaration, but must have at least one.
  auto first_var = variable();

  var_nodes.push_back(std::move(first_var));

  while (current_token_.type() == Token::Type::COMMA) {
    eat(Token::Type::COMMA);
    var_nodes.push_back(variable());
  }

  eat(Token::Type::COLON);

  auto type_node = type();

  return std::make_unique<VariableDeclaration>(std::move(var_nodes),
                                               std::move(type_node));
}

std::unique_ptr<Type> Parser::type() {
  auto token = current_token_;
  if (token.type() == Token::Type::INTEGER_TYPE) {
    eat(Token::Type::INTEGER_TYPE);
  } else {
    eat(Token::Type::REAL_TYPE);
  }
  return std::make_unique<Type>(token);
}

std::unique_ptr<Compound> Parser::compound_statement() {
  eat(Token::Type::BEGIN);
  auto nodes = statement_list();
  eat(Token::Type::END);
  auto root = std::make_unique<Compound>();
  for (auto& node : nodes) {
    root->add_child(std::move(node));
  }
  return root;
}

std::vector<std::unique_ptr<NonValueAST>> Parser::statement_list() {
  auto node = statement();
  std::vector<std::unique_ptr<NonValueAST>> results;
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

std::unique_ptr<NonValueAST> Parser::statement() {
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

std::unique_ptr<Assign> Parser::assignment_statement() {
  auto variable = this->variable();
  eat(Token::Type::ASSIGN);
  auto expr = this->expr();
  return std::make_unique<Assign>(std::move(variable), std::move(expr));
}

std::nullptr_t Parser::empty() {
  return nullptr;
}

std::unique_ptr<ValueAST> Parser::factor() {
  auto token = current_token_;
  auto type = token.type();
  switch (type) {
    case Token::Type::PLUS:
    case Token::Type::MINUS:
      eat(type);
      return std::make_unique<UnaryOperation>(factor(), token);
      break;

    case Token::Type::INTEGER_CONST:
    case Token::Type::REAL_CONST:
      eat(type);
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

std::unique_ptr<ValueAST> Parser::expr() {
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

std::unique_ptr<ValueAST> Parser::term() {
  auto node = factor();

  while (current_token_.type() == Token::Type::MULTIPLY ||
         current_token_.type() == Token::Type::INTEGER_DIV ||
         current_token_.type() == Token::Type::REAL_DIV) {
    auto token = current_token_;
    if (token.type() == Token::Type::MULTIPLY) {
      eat(Token::Type::MULTIPLY);
    } else if (token.type() == Token::Type::INTEGER_DIV) {
      eat(Token::Type::INTEGER_DIV);
    } else if (token.type() == Token::Type::REAL_DIV) {
      eat(Token::Type::REAL_DIV);
    }
    node = std::make_unique<BinaryOperation>(std::move(node), factor(), token);
  }

  return node;
}

}  // namespace Pascal
