#pragma once

#include "variable.h"

struct _vhead {
  variable_t var;
  struct _vhead* next;
};
typedef struct _vhead vhead_t;

#define vtable_hash_size 13
typedef struct {
  vhead_t* hash[vtable_hash_size];
} vtable_t;

void new_vtable(vtable_t* table);
int get_var(vtable_t* table, variable_t** var, char* name);
int add_var(vtable_t* table, variable_t* var);
void clear_vtable(vtable_t* hash);
