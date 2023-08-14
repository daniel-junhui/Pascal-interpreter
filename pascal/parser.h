// Copyright 2023 Zhu Junhui

#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ast.h"
#include "lexer.h"

namespace Pascal {

class Parser {
 private:
  Lexer lexer_;
  Token current_token_;

 public:
  template <typename T>
  requires std::convertible_to<T, std::string> explicit Parser(T&& text)
      : lexer_(std::forward<T>(text)),
        current_token_(lexer_.get_next_token()) {}

  std::unique_ptr<Program> parse();

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
 private:
  void error();

  void eat(Token::Type type);

  std::unique_ptr<Program> program();

  std::unique_ptr<Block> block();

  std::pair<std::vector<std::unique_ptr<VariableDeclaration>>,
            std::vector<std::unique_ptr<ProcedureDeclaration>>>
  declarations();

  std::unique_ptr<VariableDeclaration> variable_declaration();

  std::unique_ptr<Type> type();

  std::unique_ptr<Compound> compound_statement();

  std::vector<std::unique_ptr<NonValueAST>> statement_list();

  std::unique_ptr<NonValueAST> statement();

  std::unique_ptr<Assign> assignment_statement();

  std::nullptr_t empty();

  std::unique_ptr<ValueAST> expr();

  std::unique_ptr<ValueAST> term();

  std::unique_ptr<ValueAST> factor();

  std::unique_ptr<Variable> variable();
};

}  // namespace Pascal
