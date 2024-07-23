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
  // Not working sort
  TreeNode *tree = split(table, 3);
  return 0;
}
