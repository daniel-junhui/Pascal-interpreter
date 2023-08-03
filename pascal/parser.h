#pragma once

#include "lexer.h"
#include "AST.h"

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

  std::unique_ptr<AST> parse();

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

  std::unique_ptr<AST> program();

  std::unique_ptr<AST> compound_statement();

  std::vector<std::unique_ptr<AST>> statement_list();

  std::unique_ptr<AST> statement();

  std::unique_ptr<AST> assignment_statement();

  std::unique_ptr<Variable> variable();

  std::unique_ptr<AST> empty();

  std::unique_ptr<AST> factor();

  std::unique_ptr<AST> term();

  std::unique_ptr<AST> expr();
};

}  // namespace Pascal
