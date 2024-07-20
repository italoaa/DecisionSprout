// The data structure of the decision tree

// Basic node of the tree
typedef struct Node {
  int feature; // The feature id to split on
  double value; // The value of the feature to split on
  double threshold; // The threshold of the split
  struct Node *left; // The left child
  struct Node *right; // The right child
  int is_leaf; // Is the node a leaf
} Node;
