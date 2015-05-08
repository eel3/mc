%{
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "mc.h"

#define YYSTYPE int32_t

/* from lexer.c (generated from lexer.l) */
extern int yylex(void);
%}

%token NUMBER
%token MOD MUL POW ADD SUB DIV
%token BITAND LSHIFT RSHIFT BITXOR BITOR BITNOT
%token NOT AND OR
%token NE LT LE EQ GT GE
%token LPAR RPAR
%token EOS
%token ERROR

%left OR
%left AND
%left BITOR
%left BITXOR
%left BITAND
%left EQ NE
%left LT LE GT GE
%left LSHIFT RSHIFT
%left ADD SUB
%left MUL DIV MOD
%right MINUS
%right POW
%right PLUS NOT BITNOT

%%
list:     /* nothing */
        | list EOS
        | list expr EOS     { printf("%ld\n", (long) $2); }
        | list error EOS    { yyerrok; }
        ;

expr:     NUMBER               { $$ = $1; }
        | expr OR     expr     { $$ = $1 || $3; }
        | expr AND    expr     { $$ = $1 && $3; }
        | expr BITOR  expr     { $$ = $1 |  $3; }
        | expr BITXOR expr     { $$ = $1 ^  $3; }
        | expr BITAND expr     { $$ = $1 &  $3; }
        | expr EQ     expr     { $$ = $1 == $3; }
        | expr NE     expr     { $$ = $1 != $3; }
        | expr LT     expr     { $$ = $1 <  $3; }
        | expr LE     expr     { $$ = $1 <= $3; }
        | expr GT     expr     { $$ = $1 >  $3; }
        | expr GE     expr     { $$ = $1 >= $3; }
        | expr LSHIFT expr     { $$ = $1 << $3; }
        | expr RSHIFT expr     { $$ = $1 >> $3; }
        | expr ADD    expr     { $$ = $1 +  $3; }
        | expr SUB    expr     { $$ = $1 -  $3; }
        | expr MUL    expr     { $$ = $1 *  $3; }
        | expr DIV    expr     {
                if ($3 == 0) {
                    execerror("division by zero", NULL);
                }
                $$ = $1 /  $3; }
        | expr MOD    expr     {
                if ($3 == 0) {
                    execerror("division by zero", NULL);
                }
                $$ = $1 %  $3; }
        | SUB expr %prec MINUS { $$ = -$2; }
        | expr POW    expr     { $$ = (int32_t) pow((double) $1, (double) $3); }
        | ADD expr %prec PLUS  { $$ = $2; }
        | NOT expr             { $$ = !$2; }
        | BITNOT expr          { $$ = ~$2; }
        | LPAR expr RPAR       { $$ = $2; }
        ;
