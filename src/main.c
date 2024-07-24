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
  displayDataSet(ds, 3);

  Table *table = buildTableFromDS(ds);
  shuffleTable(table);
  encode_Labels(table, table->target->id);

  // Split the table into training and test sets
  TrTsSet *sets = splitTable(table, 0.7);
  printf("Split the Data Set into Training (%d) and Test (%d)\n", sets->train->height, sets->test->height);

  // Decide on the training set
  TreeNode *tree = decide(allocNode(sets->train));
  printf("Tree has been built using the training set\n");

  // Test the tree
  Value **predictions = predict(tree, sets->test);
  printf("Predictions have been made\n");

  // Display the predictions
  float acc = accuracy(predictions, sets->test->height ,ds);
  printf("Calculating the accuracy... \n");
  printf("Accuracy: %.2f%%\n", acc);

  freeTree(tree);
  freeTable(sets->test);
  freeDS(ds);
  return 0;
}
