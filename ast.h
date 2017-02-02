#pragma once

#include "value.h"

struct _astnode {
  int type;
  value_t value;
  int n_children;
  struct _astnode** children;
};
typedef struct _astnode astnode_t;

astnode_t* new_astnode(int type, value_t value);
void delete_astnode(astnode_t* node);
astnode_t* build_branch(astnode_t* parent, int n_children,
                        astnode_t* children[]);
