
#include "vtable.h"
#include <stdlib.h>
#include <string.h>

int hashv_var(char* name) {
  int v = 0;
  char* p = name;
  while(p) v += *(p++);
  return v % vtable_hash_size;
}

void new_vtable(vtable_t* table) {
  int i;
  for(i = 0; i < vtable_hash_size; i++)
    table->hash[i] = NULL;
}

int get_var(vtable_t* table, variable_t** var, char* name) {
  int v = hashv_var(name);
  vhead_t* n = table->hash[v];
  int found = 0;
  while(n) {
    if(strcmp(name, n->var.name) == 0) {
      found = 1;
      break;
    }
    n = n->next;
  }
  if(found && var != NULL) {
    *var = &n->var;
  }
  return found;
}

int add_var(vtable_t* table, variable_t* var) {
  int v = hashv_var(var->name);
  vhead_t* n = table->hash[v];
  vhead_t* pn = NULL;
  if(!n)
    table->hash[v] = n = malloc(sizeof(vhead_t));
  else {
    while(n) {
      // fail on double definition
      if(strcmp(var->name, n->var.name) == 0) return 0;
      pn = n;
      n = n->next;
    }
    pn->next = n = malloc(sizeof(vhead_t));
  }
  // successfully allocated now set values
  new_var(&n->var);
  copy_var(&n->var, var);
  return 1;
}

void clear_vtable(vtable_t* table) {
  int i;
  vhead_t *n, *p;
  for(i = 0; i < vtable_hash_size; i++) {
    n = table->hash[i];
    while(n) {
      p = n;
      n = n->next;
      delete_var(&p->var);
      free(p);
    }
  }
}
