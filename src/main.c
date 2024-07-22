#include <stdio.h>
#include <string.h>

#include "main.h"
#include "dataset.h"
#include "utils.h"

char *filename = "./data/Iris.csv";

int main() {
  DataSet *ds = loadCsv(filename);
  displayDataSet(ds, 25);
  return 0;
}
