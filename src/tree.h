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
  struct TreeNode *left; // Left child
  struct TreeNode *right; // Right child
  int target; // target for this node
  Split *split; // split used for this node
  Table *table; // table of values in this node
} TreeNode;


TreeNode *allocNode(Table *table);

int compare_vaules_float(const void *a, const void*b);

float *avgFeature(Value **sortedValues, int samples);

Value **sortFeature(Table *table, int feature);

Split *find_best_split(Table *table, int feature);

void split(TreeNode *node);

TreeNode *decide(TreeNode *root);

void freeTree(TreeNode *node);

Value **predict(TreeNode *root, Table *table);

Value *predictSample(TreeNode *node, Table *table, int sample);

float accuracy(Value **predictions, int pred_size, DataSet *ds);

#endif
