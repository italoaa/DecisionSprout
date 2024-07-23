#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "dataset.h"

// Sort descending
int compare_values_float(const void *a, const void *b) {
  const Value *valueA = *(const Value **)a; // Dereference to get the actual Value
  const Value *valueB = *(const Value **)b; // Dereference to get the actual Value

  if (valueA->data.f < valueB->data.f) return 1; 
  if (valueA->data.f > valueB->data.f) return -1; 
  return 0;
}

// sorts a feature
Value **sortFeature(Table *table, int feature) {
  // Data is a matrix where the first index is feature
  // The second index is the sample
  Value **data = table->data[feature];

  // Get the number of samples
  int samples = table->height;

  // Allocate memory for the new array to hold the values of the specified feature
  Value **sortedValues = (Value **)malloc(samples * sizeof(Value *));
  if (!sortedValues) {
    perror("Failed to allocate memory for sorted values");
    exit(EXIT_FAILURE);
  }

  // Copy the values from the specified feature column into the new array
  for (int i = 0; i < samples; i++) {
    sortedValues[i] = data[i];
  }

  // Quick sort
  qsort(sortedValues, samples, sizeof(Value *), compare_values_float);

  // Print the sorted values
  printf("Sorted values for feature %s\n", table->features[feature]);

  return sortedValues;
}

// Avg between datapoints
void avgFeature(Value **sortedValues, int samples) {
  // Take the i and i+1 and calculate the avg
  // replace the i with the avg
  // Leave i+1 for the next iteration

  // Average in place
  for (int i = 0; i < samples - 1; i++) {
    float avg = (sortedValues[i]->data.f + sortedValues[i + 1]->data.f) / 2;
    sortedValues[i]->data.f = avg;
  }

  // Print the averages
  printf("Averaged values\n");
}

// loop over the avgs and make a stump with the avg as threshols
// Calc GINI for each tree and pick the best
float calcGini(Table *table, float threshold, int feature) {
  printf("Calculating GINI for feature %s with threshold %f\n", table->features[feature], threshold);
  // Unique # of target classes
  Target *target = table->target;
  int targetID = target->id;

  // Each index is a class
  // Count the number of samples for that class
  // in left and right
  int leftClassCounts[target->unique];
  int rightClassCounts[target->unique];

  // Initialize the counts
  for (int i = 0; i < target->unique; i++) {
    leftClassCounts[i] = 0;
    rightClassCounts[i] = 0;
  }

  int totalLeft = 0, totalRight = 0;

  // Loop over the samples
  for (int i = 0; i < table->height; i++) {
    // Check if the sample is less than the threshold
    if (table->data[feature][i]->data.f < threshold) {
      // Increment the count for the class
      leftClassCounts[(int)table->data[targetID][i]->data.f]++;
      totalLeft++;
    } else {
      rightClassCounts[(int)table->data[targetID][i]->data.f]++;
      totalRight++;
    }
  }
 
  // print counts to test
  printf("Left counts %d\n", totalLeft);
  printf("Right counts %d\n", totalRight);

  // Calculate the GINI for the left and right
  float giniLeft = 1.0;
  float giniRight = 1.0;
  // For each class
  for (int i = 0; i < target->unique; i++) {
    // Calculate the proportion of the class for each side
    float pLeft = (float)leftClassCounts[i] / totalLeft;
    float pRight = (float)rightClassCounts[i] / totalRight;

    // Update GINI
    giniLeft -= pLeft * pLeft;
    giniRight -= pRight * pRight;
  }

  // print the GINI's
  printf("Gini left %f\n", giniLeft);
  printf("Gini right %f\n", giniRight);

  // Weighted GINI
  float gini = (totalLeft * giniLeft + totalRight * giniRight) / (totalLeft + totalRight);

  // Print the weighted GINI
  printf("Weighted GINI %f\n", gini);
  printf("=====================================\n");

  return gini;
}

// Split the data
Split *split(Table *table, int feature) {
  // Sort the feature
  printf("Splitting on feature %s\n", table->features[feature]);
  Value **thresholds = sortFeature(table, feature);

  // Calculate the averages
  avgFeature(thresholds, table->height);

  // Calculate the GINI for each threshold
  float bestGini = 1.0;
  float bestThreshold = 0.0;
  // NOTE: avgs reduce the number of thresholds by 1. (height - 1)
  for (int i = 0; i < table->height - 1; i++) {
    float threshold = thresholds[i]->data.f;
    float gini = calcGini(table, threshold, feature);
    if (gini < bestGini) {
      bestGini = gini;
      bestThreshold = threshold;
    }
  }

  // Print the best threshold
  printf("Best threshold %f\n", bestThreshold);
  printf("Best GINI %f\n", bestGini);

  Split *split = (Split *)malloc(sizeof(Split));
  if (!split) {
    perror("Failed to allocate memory for split");
    exit(EXIT_FAILURE);
  }

  split->feature = feature;
  split->threshold = bestThreshold;
  split->gini = bestGini;

  return split;
}

TreeNode *decide(Table *table) {
  // Loop over all features
  /* for (int i = 0; i < table->width; i++) { */
    // Split the data
    /* Split *split = split(table, i); */
  /* } */

  // For now just split on the 3 feature
  /* Split *split = split(table, 3); */
  /* printf("Split: %d\n", split->feature); */
  /* printf("Split: %f\n", split->threshold); */
  /* printf("Split: %f\n", split->gini); */

  return NULL;
}
