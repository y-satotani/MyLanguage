
#include "reg.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int hashv_reg(astnode_t* node) {
  return (uintptr_t)node % regset_hash_size;
}

void new_regset(regset_t* set, int nreg) {
  int i;
  for(i = 0; i < regset_hash_size; i++)
    set->hash[i] = NULL;
  set->used = (int*) malloc(sizeof(int) * nreg);
  set->nreg = nreg;
  for(i = 0; i < nreg; i++)
    set->used[i] = 0;
  set->next = 0;
}

int get_reg(regset_t* set, astnode_t* node) {
  int v = hashv_reg(node);
  reghead_t* n = set->hash[v];
  int found = 0;
  while(n) {
    if(node == n->node)
      return n->reg;
    n = n->next;
  }
  return -1;
}

int regset_add(regset_t* set, astnode_t* node) {
  int v = hashv_reg(node);
  reghead_t* n = set->hash[v];
  reghead_t* pn = NULL;
  // the register table is full
  if(set->next < 0) return -1;
  if(!n)
    set->hash[v] = n = malloc(sizeof(reghead_t));
  else {
    while(n) {
      // already assigned
      if(node == n->node) return -1;
      pn = n;
      n = n->next;
    }
    pn->next = n = malloc(sizeof(reghead_t));
  }
  // successfully allocated now set values
  n->node = node;
  n->reg = set->next;
  n->next = NULL;
  set->used[set->next] = 1;
  for(v = 1; v < set->nreg; v++) {
    if(set->used[(set->next + v) % set->nreg] == 0) {
      set->next = (set->next + v) % set->nreg;
      break;
    }
  }
  // didn't run break statement, table is full
  if(v == set->nreg) set->next = -1;
  return n->reg;
}

int remove_reg(regset_t* set, astnode_t* node) {
  int v = hashv_reg(node);
  int success = 0;
  reghead_t* n = set->hash[v];
  reghead_t* pn = NULL;
  if(!n) return 0;
  while(n) {
    if(node == n->node) {
      success = 1;
      break;
    }
    pn = n;
    n = n->next;
  }
  if(success) {
    set->next = n->reg;
    set->used[n->reg] = 0;
    if(pn) pn->next = n->next;
    else set->hash[v] = NULL;
    free(n);
  }
  return success;
}

void clear_regset(regset_t* set) {
  int i;
  reghead_t *n, *p;
  for(i = 0; i < regset_hash_size; i++) {
    n = set->hash[i];
    while(n) {
      p = n;
      n = n->next;
      free(p);
    }
    set->hash[i] = 0;
  }
  for(i = 0; i < set->nreg; i++) {
    set->used[i] = 0;
  }
  set->next = 0;
}

void delete_regset(regset_t* set) {
  clear_regset(set);
  set->nreg = 0;
  free(set->used);
  set->used = 0;
}
