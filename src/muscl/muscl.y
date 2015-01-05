/*
 * $Id: muscl.y 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%{

#include <stdio.h>
#include <string>
#include "vm.h"
#include "ast.h"
#include "ast-assignment.h"
#include "ast-binary.h"
#include "ast-conditional.h"
#include "ast-declaration.h"
#include "ast-expr-list.h"
#include "ast-func-call.h"
#include "ast-func-decl.h"
#include "ast-jump.h"
#include "ast-list.h"
#include "ast-loop.h"
#include "ast-map-pair.h"
#include "ast-map-list.h"
#include "ast-map.h"
#include "ast-object.h"
#include "ast-object-read.h"
#include "ast-object-write.h"
#include "ast-param-list.h"
#include "ast-reference.h"
#include "ast-send.h"
#include "ast-stmt-list.h"
#include "ast-unary.h"
#include "ast-value.h"
#include "cl.h"
#include "datum.h"
#include "scope.h"
#include "symbol.h"

#define YYSTYPE Ast*

// Stuff for Flex & Bison...
int yylex (void);
extern char* yytext;
extern int yylineno;

// Stuff for me...
Scope scope;
Ast* ast_root;
std::string compile_errors;
VirtualMachine bones;
ContextLayer context;

void yyerror(char const* s) {
  char* foo = new char[strlen(s)+100];
  sprintf(foo, "muscl: error on line %d: %s\n", yylineno, s);
  compile_errors = foo;
  delete [] foo;
  throw compile_errors;
  return;
}

std::string decl_info(Symbol* sym) {
  char buf[256];
  sprintf(buf, "it was declared on line %d as a %s", sym->line(), sym->stringType());
  return std::string(buf);
}

AstReference* assertCreate(char* token, const unsigned& type, const char* strtype) {
  char buf[256];
  Symbol* sym = scope.lookup(token);
  if (sym) {
    sprintf(buf, "Cannot redeclare '%s' as a %s (%s)", token, strtype, decl_info(sym).c_str());
    yyerror(buf);
  }
  sym = new Symbol(token, type, yylineno);
  scope.insert(sym);
  return new AstReference(sym);
}

AstReference* assertLookup(char* token) {
  char buf[256];
  Symbol* sym = scope.lookup(token);
  if (!sym) {
    sprintf(buf, "Attempting to use undefined symbol '%s' on line %d", token, yylineno);
    yyerror(buf);
  }
  return new AstReference(sym);
}

void assertVariable(AstReference* ref) {
  if (!ref->symbol()->isVariable()) {
    std::string err("Cannot mutate '");
    err.append(ref->symbol()->name());
    err.append("' (").append(decl_info(ref->symbol())).append(").");
    yyerror(err.c_str());
  }
  return;
}

void assertFunction(AstReference* ref) {
  if (!ref->symbol()->isFunction()) {
    std::string err("Cannot call '");
    err.append(ref->symbol()->name());
    err.append("' as function (").append(decl_info(ref->symbol())).append(").");
    yyerror(err.c_str());
  }
  return;
}

AstObject* assertObject(AstObject* object) {
  if (!context.isValidReference(object->fullName())) {
    std::string err(object->fullName());
    err.append(" is not a valid object reference.");
    yyerror(err.c_str());
  }
  return object;
}

void assertObjectRead(AstObject* object) {
  if (!context.readable(object->fullName())) {
    std::string err(object->fullName());
    err.append(" is not a readable object.");
    yyerror(err.c_str());
  }
  return;
}

void assertObjectWrite(AstObject* object) {
  if (!context.writable(object->fullName())) {
    std::string err(object->fullName());
    err.append(" is not a writable object.");
    yyerror(err.c_str());
  }
  return;
}

%}

%token IDENTIFIER
%token FUNCTION
%token DOLLAR COLON ARROW
%token INT REAL STRING CONST VAR
%token OP_LE OP_GE OP_EQ OP_NE
%token AND OR NOT
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN POW_ASSIGN
%token IF UNLESS ELSE WHILE UNTIL DO FOR IN FROM TO
%token CONTINUE BREAK RETURN
%token SEND
%token SEMICOLON L_BRACE R_BRACE COMMA EQUAL L_PAREN R_PAREN L_BRACKET R_BRACKET MINUS PLUS STAR SLASH MODULUS CARAT LT GT PIPE

%%

script
  : statement_list                                            { ast_root = $1; }
  ;

statement_list
  : statement                                                 { $$ = new AstStatementList($1, NULL); }
  | statement statement_list                                  { $$ = new AstStatementList($1, (AstStatementList*)$2); }
  ;

statement
  : declaration                                               { $$ = $1; }
  | assignment SEMICOLON                                      { $$ = $1; }
  | conditional_statement                                     { $$ = $1; }
  | loop_statement                                            { $$ = $1; }
  | jump_statement SEMICOLON                                  { $$ = $1; }
  | function_call SEMICOLON                                   { $$ = $1; }
  | send_statement SEMICOLON                                  { $$ = $1; }
  ;

conditional_statement
  : IF L_PAREN expression R_PAREN block                       { $$ = new AstConditional(AST_CONT_IF, $3, $5, NULL); }
  | IF L_PAREN expression R_PAREN block ELSE block            { $$ = new AstConditional(AST_CONT_IF_ELSE, $3, $5, $7); }
  | UNLESS L_PAREN expression R_PAREN block                   { $$ = new AstConditional(AST_CONT_UNLESS, $3, $5, NULL); }
  ;

block
  : L_BRACE statement_list R_BRACE                            { $$ = $2; }
  ;

loop_statement
  : WHILE L_PAREN expression R_PAREN block                    { $$ = new AstLoop(AST_CONT_WHILE, NULL, $3, NULL, $5); }
  | DO block WHILE L_PAREN expression R_PAREN                 { $$ = new AstLoop(AST_CONT_DO_WHILE, NULL, $5, NULL, $2); }
  | UNTIL L_PAREN expression R_PAREN block                    { $$ = new AstLoop(AST_CONT_UNTIL, NULL, $3, NULL, $5); }
  | DO block UNTIL L_PAREN expression R_PAREN                 { $$ = new AstLoop(AST_CONT_DO_UNTIL, NULL, $5, NULL, $2); }
  | FOR     id_lookup     FROM expression TO expression block { $$ = new AstLoop(AST_CONT_FOR_TO, (AstReference*)$2, $4, $6, $7); }
  | FOR VAR id_create_var FROM expression TO expression block { $$ = new AstLoop(AST_CONT_FOR_TO, (AstReference*)$3, $5, $7, $8); }
  | FOR     id_lookup     IN expression block                 { $$ = new AstLoop(AST_CONT_FOR_IN, (AstReference*)$2, $4, NULL, $5); }
  | FOR VAR id_create_var IN expression block                 { $$ = new AstLoop(AST_CONT_FOR_IN, (AstReference*)$3, $5, NULL, $6); }
  ;

jump_statement
  : CONTINUE                                                  { $$ = new AstJump(AST_JUMP_CONT); }
  | BREAK                                                     { $$ = new AstJump(AST_JUMP_BREAK); }
  | RETURN                                                    { $$ = new AstJump(AST_FUNC_RETURN); }
  | RETURN expression                                         { $$ = new AstJump(AST_FUNC_RETURN, $2); }
  ;

function_call
  : id_lookup L_PAREN expression_list R_PAREN {
      assertFunction((AstReference*)$1);
      $$ = new AstFunctionCall((AstReference*)$1, (AstExpressionList*)$3);
    }
  ;

parameter_list
  : /* nothing */                                             { $$ = new AstParameterList(NULL, NULL); }
  | id_create_var                                             { $$ = new AstParameterList((AstReference*)$1, NULL); }
  | id_create_var COMMA parameter_list                        { $$ = new AstParameterList((AstReference*)$1, (AstParameterList*)$3); }
  ;

id_create_var
  : IDENTIFIER                                                { $$ = assertCreate(yytext, SYMBOL_VAR, "variable"); }
  ;

id_create_const
  : IDENTIFIER                                                { $$ = assertCreate(yytext, SYMBOL_CONST, "constant"); }
  ;

id_create_func
  : IDENTIFIER                                                { $$ = assertCreate(yytext, SYMBOL_FUNC, "function"); }
  ;

id_lookup
  : IDENTIFIER                                                { $$ = assertLookup(yytext); }
  ;

send_statement
  : SEND expression                                           { $$ = new AstSend($2); }
  ;

declaration
  : CONST id_create_const EQUAL expression SEMICOLON          { $$ = new AstDeclaration(AST_CONST, (AstReference*)$2, $4); }
  | VAR id_create_var EQUAL expression SEMICOLON              { $$ = new AstDeclaration(AST_VAR, (AstReference*)$2, $4); }
  | FUNCTION id_create_func {
      char buf[256];
      sprintf(buf, "user-func-%s", ((AstReference*)$2)->symbol()->name().c_str());
      scope.pushFrame(buf);
    }
    L_PAREN parameter_list R_PAREN block {
      $$ = new AstFunctionDeclaration((AstReference*)$2, (AstParameterList*)$5, $7, scope.popFrame());
    }
  ;

assignment
  : id_lookup EQUAL expression                                { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS,     (AstReference*)$1, $3); }
  | id_lookup ADD_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_ADD, (AstReference*)$1, $3); }
  | id_lookup SUB_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_SUB, (AstReference*)$1, $3); }
  | id_lookup MUL_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_MUL, (AstReference*)$1, $3); }
  | id_lookup DIV_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_DIV, (AstReference*)$1, $3); }
  | id_lookup MOD_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_MOD, (AstReference*)$1, $3); }
  | id_lookup POW_ASSIGN expression                           { assertVariable((AstReference*)$1); $$ = new AstAssignment(AST_ASS_POW, (AstReference*)$1, $3); }
  | object    EQUAL expression                                { assertObjectWrite((AstObject*)$1); $$ = new AstObjectWrite((AstObject*)$1, $3); }
  ;

primary_expression
  : id_lookup                                                 { $$ = $1; }
  | constant_value                                            { $$ = $1; }
  | L_PAREN expression R_PAREN                                { $$ = $2; }
  | list_initialization                                       { $$ = $1; }
  | map_initialization                                        { $$ = $1; }
  | function_call                                             { $$ = $1; }
  | object                                                    { assertObjectRead((AstObject*)$1); $$ = new AstObjectRead((AstObject*)$1); }
  ;

constant_value
  : INT {
      long l = 0;
      sscanf(yytext, "%ld", &l);
      $$ = new AstValue(new Datum(l));
    }
  | REAL {
      double d = 0;
      sscanf(yytext, "%lf", &d);
      $$ = new AstValue(new Datum(d));
    }
  | STRING {
      /* the witch-trickery here is to efficiently trim the double quotes off of yytext */
      yytext[strlen(yytext)-1] = 0;
      $$ = new AstValue(new Datum(yytext+1));
    }
  ;

list_initialization
  : L_BRACKET expression_list R_BRACKET                       { $$ = new AstList((AstExpressionList*)$2); }
  ;

map_initialization
  : L_BRACE map_list R_BRACE                                  { $$ = new AstMap((AstMapList*)$2); }
  ;

object
  : object_reference                                          { $$ = assertObject((AstObject*)$1); }
  ;

object_reference
  : DOLLAR object_name ARROW more_objects                     { ((AstObject*)$2)->addChild((AstObject*)$4); $$ = $2; }
  ;

more_objects
  : object_name                                               { $$ = $1; }
  | object_name ARROW more_objects                            { ((AstObject*)$1)->addChild((AstObject*)$3); $$ = $1; }
  ;

object_name
  : IDENTIFIER                                                { $$ = new AstObject(yytext); }
  ;

expression_list
  : /* nothing */                                             { $$ = new AstExpressionList(NULL, NULL); }
  | expression                                                { $$ = new AstExpressionList($1, NULL); }
  | expression COMMA expression_list                          { $$ = new AstExpressionList($1, (AstExpressionList*)$3); }
  ;

map_list
  : /* nothing */                                             { $$ = new AstMapList(NULL, NULL); }
  | map_pair                                                  { $$ = new AstMapList($1, NULL); }
  | map_pair COMMA map_list                                   { $$ = new AstMapList($1, (AstMapList*)$3); }
  ;

map_pair
  : expression COLON expression                               { $$ = new AstMapPair($1, $3); }
  ;

unary_expression
  : primary_expression                                        { $$ = $1; }
  | MINUS unary_expression                                    { $$ = new AstUnaryOperation(AST_UN_MIN, $2); }
  | NOT unary_expression                                      { $$ = new AstUnaryOperation(AST_UN_NOT, $2); }
  | PIPE unary_expression PIPE                                { $$ = new AstUnaryOperation(AST_UN_CARD, $2); }
  ;

exponential_expression
  : unary_expression                                          { $$ = $1; }
  | exponential_expression CARAT unary_expression             { $$ = new AstBinaryOperation(AST_BIN_POW, $1, $3); }
  ;

multiplicative_expression
  : exponential_expression                                    { $$ = $1; }
  | multiplicative_expression STAR exponential_expression     { $$ = new AstBinaryOperation(AST_BIN_MUL, $1, $3); }
  | multiplicative_expression SLASH exponential_expression    { $$ = new AstBinaryOperation(AST_BIN_DIV, $1, $3); }
  | multiplicative_expression MODULUS exponential_expression  { $$ = new AstBinaryOperation(AST_BIN_MOD, $1, $3); }
  ;

additive_expression
  : multiplicative_expression                                 { $$ = $1; }
  | additive_expression PLUS multiplicative_expression        { $$ = new AstBinaryOperation(AST_BIN_ADD, $1, $3); }
  | additive_expression MINUS multiplicative_expression       { $$ = new AstBinaryOperation(AST_BIN_SUB, $1, $3); }
  ;

relational_expression
  : additive_expression                                       { $$ = $1; }
  | relational_expression LT additive_expression              { $$ = new AstBinaryOperation(AST_BIN_LT, $1, $3); }
  | relational_expression GT additive_expression              { $$ = new AstBinaryOperation(AST_BIN_GT, $1, $3); }
  | relational_expression OP_LE additive_expression           { $$ = new AstBinaryOperation(AST_BIN_LE, $1, $3); }
  | relational_expression OP_GE additive_expression           { $$ = new AstBinaryOperation(AST_BIN_GE, $1, $3); }
  ;

equality_expression
  : relational_expression                                     { $$ = $1; }
  | equality_expression OP_EQ relational_expression           { $$ = new AstBinaryOperation(AST_BIN_EQ, $1, $3); }
  | equality_expression OP_NE relational_expression           { $$ = new AstBinaryOperation(AST_BIN_NE, $1, $3); }
  ;

logical_and_expression
  : equality_expression                                       { $$ = $1; }
  | logical_and_expression AND equality_expression            { $$ = new AstBinaryOperation(AST_BIN_AND, $1, $3); }
  ;

logical_or_expression
  : logical_and_expression                                    { $$ = $1; }
  | logical_or_expression OR logical_and_expression           { $$ = new AstBinaryOperation(AST_BIN_OR, $1, $3); }
  ;

expression
  : logical_or_expression                                     { $$ = $1; }
  ;

%%

/*
 * Note to self: Set yyin to a FILE* to override the input for yyparse().
 */

#include "program.h"
int main(void) {
/*
Program p("easy");
scope.pushFrame("built-in");  // namespace for system functions and constants
scope.pushFrame("global");    // default namespace for programs
if (p.source("send \"hello, world!\";")) {
  printf("looks good\n");
  std::string errors;
  if (p.compile(errors)) {
    printf("it compiled??\n");
  } else {
    printf("figures: %s\n", errors.c_str());
  }
} else {
  printf("better luck next time!\n");
}
//*/
/*
Datum number(5);
Datum real(3.1415);
Datum str("stuff");
Datum array(number);
array.append(real);
array.append(str);
array.append(array);
printf("number: '%s'\n", number.serialize().c_str());
printf("real  : '%s'\n", real.serialize().c_str());
printf("string: '%s'\n", str.serialize().c_str());
printf("array : '%s'\n", array.serialize().c_str());

char* d = "a654xyz";
char a;
int b;
char c[10];
printf("%s\n", d);
char s[100];
sscanf(d, "%c%d%s", &a, &b, c);
printf("orig: %s\nfirst: %c\nlen: %d\ncontent: %s\n", d, a, b, c);

unser("i503");
unser("r3.141592");
unser("s5:stuff");
unser("l3i503r3.14s2:yo");
//*/
//*
  ast_root = NULL;
  scope.pushFrame("built-in");  // namespace for system functions and constants
  scope.pushFrame("global");    // default namespace for programs
  try {
    yyparse();
    ast_root->print("", true);
    printf("\n");
    ast_root->codegen();
    ast_root->dumpcode();
    bones.run(&ast_root->code());
  } catch (std::string err) {
    printf("%s\n", err.c_str());
  }
  delete ast_root;
//*/
}
