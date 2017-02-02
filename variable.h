#pragma once

typedef struct {
  char* name;
  int offset;
  int n_dimen;
  int* dimens;
  int init_val;
} variable_t;

void new_var(variable_t* var);
void delete_var(variable_t* var);
void copy_var(variable_t* v, variable_t* w);
void add_dimen(variable_t* var, int size);
