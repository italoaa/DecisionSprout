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

  // Split the table into training and test sets
  TrTsSet *sets = splitTable(table, 0.7);

  // Decide on the training set
  TreeNode *tree = decide(allocNode(sets->train));

  // Test the tree
  Value **predictions = predict(tree, sets->test);

  // Display the predictions
  float acc = accuracy(predictions, sets->test->height ,ds);
  printf("Accuracy: %f\n", acc);

  freeTree(tree);
  freeTable(sets->test);
  freeDS(ds);
  return 0;
}
