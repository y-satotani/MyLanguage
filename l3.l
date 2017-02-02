%{
#include <string.h>
#include "l3.h"
#include "l3.tab.h"
%}
%%
define	return KW_DEFINE;
func	return KW_FUNC;
if	return KW_IF;
else	return KW_ELSE;
while	return KW_WHILE;
for	return KW_FOR;

"+"	return OP_ADD;
"-"	return OP_SUB;
"*"	return OP_MUL;
"/"	return OP_DIV;

"++"	return OP_INC;
"--"	return OP_DEC;

"=="	return OP_EQ;
">"	return OP_GT;
">="	return OP_GE;
"<"	return OP_LT;
"<="	return OP_LE;

"="	return OP_ASSG;

";"	return STAT_TERM;

[a-zA-Z][a-zA-Z0-9]* {
  yylval.as_s = strdup(yytext);
  return ID;
}

[0-9]+ {
  yylval.as_i = atoi(yytext);
  return NUM;
}

[ \t\n]+
. return yytext[0];
%%
