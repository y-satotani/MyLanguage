#include "l3.h"
#include "l3.tab.h"

#define YY_USER_INIT yyin = stdin; yyout = stdout;

int main(int argc, char* argv[]) {
  return yyparse();
}
