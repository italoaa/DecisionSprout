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


TreeNode *allocNode(Table *table);

int compare_vaules_float(const void *a, const void*b);

float *avgFeature(Value **sortedValues, int samples);

Value **sortFeature(Table *table, int feature);

Split *find_best_split(Table *table, int feature);

TreeNode *decide(Table *table);

#endif
