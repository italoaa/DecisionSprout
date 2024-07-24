#include <stdio.h>
#include <string.h>

#include "main.h"
#include "tree.h"
#include "dataset.h"
#include "utils.h"

char *filename = "./data/Iris.csv";

int main() {
  DataSet *ds = loadCsv(filename);
  setTarget(ds, 5);
  displayHeader(ds);
  Table *table = buildTableFromDS(ds);
  encode_Labels(table, table->target->id);

  /* Decision tree */
  TreeNode *tree = decide(allocNode(table));
  export_tree_to_dot(tree, "tree.dot");
  printf("Decision tree exported to tree.dot\n");

  freeTree(tree);
  freeDS(ds);
  return 0;
}
