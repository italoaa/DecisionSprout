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
  Table *table = buildTableFromIds(ds, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
  encode_Labels(table, table->target->id);

  // Decision tree
  TreeNode *tree = decide(allocNode(table));
  return 0;
}
