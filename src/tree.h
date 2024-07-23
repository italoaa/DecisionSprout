// Tree

#ifndef TREE_H
#define TREE_H

#include "dataset.h"

typedef struct Split {
  int feature;
  float threshold;
  float gini;
} Split;

typedef struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  Split *split;
  Table *table;
} TreeNode;



int compare_vaules_float(const void *a, const void*b);

Value **sortFeature(Table *table, int feature);

Split *best_split(Table *table, int feature);

TreeNode *decide(Table *table);

#endif
