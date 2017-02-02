
#include "variable.h"
#include <stdlib.h>
#include <string.h>

void new_var(variable_t* var) {
  var->n_dimen = 0;
  var->dimens = NULL;
  var->init_val = 0;
}

void delete_var(variable_t* var) {
  var->n_dimen = 0;
  free(var->dimens);
  var->dimens = NULL;
}

void copy_var(variable_t* v, variable_t* w) {
  v->name = w->name;
  v->offset = w->offset;
  v->n_dimen = w->n_dimen;
  v->dimens = malloc(sizeof(int)*w->n_dimen);
  memcpy(v->dimens, w->dimens, sizeof(int)*w->n_dimen);
  v->init_val = w->init_val;
}

void add_dimen(variable_t* var, int size) {
  var->n_dimen++;
  var->dimens = realloc(var->dimens, sizeof(int) * var->n_dimen);
  var->dimens[var->n_dimen-1] = size;
}
