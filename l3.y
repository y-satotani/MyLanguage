%{
#include "value.h"
#include "ast.h"
#include "l3.h"
#define __v(vt, f, v)              \
  (value_t){vt,{.f=v}}
#define __nn(nt, v)                \
  new_astnode(nt, v)
#define __bb(p, nc, ...)           \
  build_branch(p, nc,              \
    (astnode_t*[]){__VA_ARGS__})
%}
%union {
  int as_i;
  char as_c;
  char* as_s;
  struct _value* as_v;
  struct _astnode* as_p;
}
%token <as_s> ID
%token <as_i> NUM
%token KW_DEFINE KW_FUNC KW_IF KW_WHILE KW_FOR
%right ')' KW_ELSE
%token <as_i> OP_ADD OP_SUB OP_MUL OP_DIV
%token <as_i> OP_EQ OP_GT OP_GE OP_LT OP_LE
%token <as_i> OP_ASSG
%token <as_i> OP_INC OP_DEC
%token STAT_TERM
%type <as_p> programme statements statement
%type <as_p> definition defs def
%type <as_p> expression expr_assg expr_cond
%type <as_p> expr_alge term factor
%type <as_i> comparator addsub muldiv incdec
%type <as_p> value variable subscript subscripts
%type <as_p> conditional loop
%type <as_p> function argvars argvar argsubsc argvals argval 
%%
programme : statements
statements : statements statement { $$ = __bb(__nn(NT_STATEMENTS, val_null), 2, $1, $2); }
           | statement            { $$ = $1; }

statement : definition STAT_TERM { $$ = $1; }
          | expression STAT_TERM { $$ = $1; }
          | conditional          { $$ = $1; }
          | loop                 { $$ = $1; }
          | function             { $$ = $1; }
          | '{' statements '}'   { $$ = $2; }
          | STAT_TERM            { $$ = __nn(NT_STATEMENT, val_null); }

definition : KW_DEFINE defs { $$ = __bb(__nn(NT_DEFINITION, val_null), 1, $2); }

defs : defs ',' def { $$ = __bb(__nn(NT_DEFS, val_null), 2, $1, $3); }
     | def          { $$ = $1; }

def : variable OP_ASSG expr_cond { $$ = __bb(__nn(NT_DEF, val_null), 2, $1, $3); }
    | variable                   { $$ = __bb(__nn(NT_DEF, val_null), 1, $1); }

expression : expr_assg { $$ = __bb(__nn(NT_EXPRESSION, val_null), 1, $1); }

expr_assg : variable OP_ASSG expr_assg { $$ = __bb(__nn(NT_EXPR_ASSG, val_null), 2, $1, $3); }
          | expr_cond                  { $$ = $1; }

expr_cond : expr_alge comparator expr_alge { $$ = __bb(__nn(NT_EXPR_COND, __v(integer_t, as_i, $2)), 2, $1, $3); }
          | expr_alge                      { $$ = $1; }

expr_alge : expr_alge addsub term { $$ = __bb(__nn(NT_EXPR_ALGE, __v(integer_t, as_i, $2)), 2, $1, $3); }
          | term                  { $$ = $1; }

term : term muldiv factor { $$ = __bb(__nn(NT_TERM, __v(integer_t, as_i, $2)), 2, $1, $3); }
     | factor             { $$ = $1; }

factor : ID '(' argvals ')' { $$ = __bb(__nn(NT_FACTOR, __v(string_t, as_s, $1)), 1, $3); }
       | incdec value       { $$ = __bb(__nn(NT_FACTOR, __v(integer_t, as_i, $1)), 2, __nn(0, val_null), $2); }
       | value incdec       { $$ = __bb(__nn(NT_FACTOR, __v(integer_t, as_i, $2)), 2, $1, __nn(0, val_null)); }
       | '(' expression ')' { $$ = $2; }
       | value              { $$ = $1; }

addsub : OP_ADD | OP_SUB
muldiv : OP_MUL | OP_DIV
comparator : OP_EQ | OP_GT | OP_LT | OP_GE | OP_LE
incdec : OP_INC | OP_DEC

conditional : KW_IF '(' expression ')' statement       	       	   { $$	= __bb(__nn(NT_CONDITIONAL, val_null), 2, $3, $5); }
            | KW_IF '(' expression ')' statement KW_ELSE statement { $$ = __bb(__nn(NT_CONDITIONAL, val_null), 3, $3, $5, $7); }

loop : KW_FOR '(' expression STAT_TERM expression STAT_TERM expression ')' statement { $$ = __bb(__nn(NT_LOOP, val_null), 4, $3, $5, $7, $9); }
     | KW_WHILE '(' expression ')' statement                                         { $$ = __bb(__nn(NT_LOOP, val_null), 2, $3, $5); }

function : KW_FUNC ID '(' argvars ')' statement { $$ = __bb(__nn(NT_FUNCTION, __v(string_t, as_s, $2)), 2, $4, $6); }
argvars : %empty             { $$ = __nn(NT_ARGVARS, val_null); }
        | argvar             { $$ = $1; }
        | argvars ',' argvar { $$ = __bb(__nn(NT_ARGVARS, val_null), 2, $1, $3); }
argvar : ID          { $$ = __nn(NT_ARGVAR, __v(string_t, as_s, $1)); }
       | ID argsubsc { $$ = __bb(__nn(NT_ARGVAR, __v(string_t, as_s, $1)), 1, $2); }
argsubsc : '[' ']'            { $$ = __nn(NT_ARGSUBSC, val_null); }
         | subscripts         { $$ = $1; }
         | '[' ']' subscripts { $$ = __bb(__nn(NT_ARGSUBSC, val_null), 1, $3); }
argvals	: argval             { $$ = $1; }
       	| argvals ',' argval { $$ = __bb(__nn(NT_ARGVALS, val_null), 2, $1, $3); }
argval : expression          { $$ = __bb(__nn(NT_ARGVAL, val_null), 1, $1); }

variable : ID            { $$ = __nn(NT_VARIABLE, __v(string_t, as_s, $1)); }
         | ID subscripts { $$ = __bb(__nn(NT_VARIABLE, __v(string_t, as_s, $1)), 1, $2); }

value : variable { $$ = __bb(__nn(NT_VALUE, val_null), 1, $1); }
      | NUM	 { $$ = __nn(NT_VALUE, __v(integer_t, as_i, $1)); }

subscripts : subscript            { $$ = $1; }
           | subscripts subscript { $$ = __bb(__nn(NT_SUBSCRIPTS, val_null), 2, $1, $2); }
subscript  : '[' expression ']'   { $$ = __bb(__nn(NT_SUBSCRIPT, val_null), 1, $2); }

%%
