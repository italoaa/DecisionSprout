#include <stdio.h>
#include <string.h>

#include "main.h"
#include "dataset.h"
#include "utils.h"
#include "tree.h"

char *filename = "./data/Iris.csv";

int main() {
  DataSet *ds = loadCsv(filename);
  displayHeader(ds);
  displayDataSet(ds, 4);
  Table *table = buildTableFromIds(ds, (int[]){0, 1, 2, 3}, 4);
  displayTable(table, 4);
  // Not working sort
  /* sortValues(table, 3); */
  return 0;
}
