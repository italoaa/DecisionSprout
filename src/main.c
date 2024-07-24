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
  shuffleTable(table);
  encode_Labels(table, table->target->id);

  TrTsSet *sets = splitTable(table, 0.7);

  // Decide on the training set
  TreeNode *tree = decide(allocNode(sets->train));
  /* export_tree_to_dot(tree, "tree.dot"); */
  /* printf("Decision tree exported to tree.dot\n"); */

  /* freeTree(tree); */
  freeDS(ds);
  return 0;
}
