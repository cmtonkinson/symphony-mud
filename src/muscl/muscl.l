

%{
#include "muscl.tab.h"

%}

%option noyywrap yylineno

%%

"and"       { return AND; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"const"     { return CONST; }
"do"        { return DO; }
"else"      { return ELSE; }
"for"       { return FOR; }
"from"      { return FROM; }
"function"  { return FUNCTION; }
"if"        { return IF; }
"in"        { return IN; }
"not"       { return NOT; }
"or"        { return OR; }
"return"    { return RETURN; }
"send"      { return SEND; }
"to"        { return TO; }
"unless"    { return UNLESS; }
"until"     { return UNTIL; }
"var"       { return VAR; }
"while"     { return WHILE; }

[a-z_][a-z0-9_]*    { return IDENTIFIER; }
[0-9]+              { return INT; }
[0-9]*"."[0-9]+     { return REAL; }
\"(?:[^\"]|\\\")+\" { return STRING; }

"$"       { return DOLLAR; }
":"       { return COLON; }

"+="      { return ADD_ASSIGN; }
"-="      { return SUB_ASSIGN; }
"*="      { return MUL_ASSIGN; }
"/="      { return DIV_ASSIGN; }
"%="      { return MOD_ASSIGN; }
"^="      { return POW_ASSIGN; }

"&&"      { return AND; }
"||"      { return OR; }
"!"       { return NOT; }

"<="      { return OP_LE; }
">="      { return OP_GE; }
"=="      { return OP_EQ; }
"!="      { return OP_NE; }

"->"      { return ARROW; }
";"       { return SEMICOLON; }
"{"       { return L_BRACE; }
"}"       { return R_BRACE; }
","       { return COMMA; }
"="       { return EQUAL; }
"("       { return L_PAREN; }
")"       { return R_PAREN; }
"["       { return L_BRACKET; }
"]"       { return R_BRACKET; }
"-"       { return MINUS; }
"+"       { return PLUS; }
"*"       { return STAR; }
"/"       { return SLASH; }
"%"       { return MODULUS; }
"^"       { return CARAT; }
"<"       { return LT; }
">"       { return GT; }
"|"       { return PIPE; }

\/\*([^\*]|\*[^/])+\*\/ { /* block comment */ }
\/\/[^\n]*              { /* line comment */ }
[ \n\t]+                { /* whitespace */ }
.                       { fprintf(stderr, "wtf: \"%c\"\n", yytext[0]); }

%%
