#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

astnode_t* new_astnode(int type, value_t value) {
  astnode_t* node = (astnode_t*)malloc(sizeof(astnode_t));
  node->type = type;
  node->value = value;
  node->n_children = 0;
  node->children = NULL;
  return node;
}

void delete_astnode(astnode_t* node) {
  int i;
  for(i = 0; i < node->n_children; i++)
    delete_astnode(node->children[i]);
  if(node->children)
    free(node->children);
  node->children = NULL;
  free(node);
}

astnode_t* build_branch(astnode_t* parent, int n_children,
                        astnode_t* children[]) {
  int i;
  parent->n_children = n_children;
  parent->children = (astnode_t**)malloc(sizeof(astnode_t*)*n_children);
  for(i = 0; i < n_children; i++)
    parent->children[i] = children[i];
  return parent;
}
