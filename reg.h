#pragma once

#include "ast.h"

struct _reghead {
  astnode_t* node;
  int reg;
  struct _reghead* next;
};
typedef struct _reghead reghead_t;

#define regset_hash_size 13
typedef struct {
  reghead_t* hash[regset_hash_size];
  int nreg;
  int* used;
  int next;
} regset_t;

void new_regset(regset_t* set, int nreg);
int get_reg(regset_t* set, astnode_t* node);
int add_reg(regset_t* set, astnode_t* node);
int remove_reg(regset_t* set, astnode_t* node);
void clear_regset(regset_t* set);
void delete_regset(regset_t* set);
