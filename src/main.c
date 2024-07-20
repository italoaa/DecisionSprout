#include <stdio.h>
#include <string.h>

#include "main.h"
#include "dataset.h"
#include "utils.h"

const char *filename = "./data/Iris.csv";

DataSet *ds;


int main() {
  DataSet *ds = loadCsv(filename);
  return 0;
}
