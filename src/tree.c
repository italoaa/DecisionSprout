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

  for (int i = 0; i < samples; i++) {
    printf("%f\n", sortedValues[i]->data.f);
  }

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
  printf("Averages\n");
  for (int i = 0; i < samples - 1; i++) {
    printf("%f\n", sortedValues[i]->data.f);
  }
}

// loop over the avgs and make a stump with the avg as threshols
// Calc GINI for each tree and pick the best
/* float calcGini(Table *table, float threshold, int feature) { */
/*   // Unique # of target classes */

/*   // Each index is a class */
/*   // Count the number of samples for that class */
/*   // in left and right */
/*   int leftClassCounts[numClasses]; */
/*   int rightClassCounts[numClasses]; */

/*   int totalLeft = 0, totalRight = 0; */

/*   // Loop over the samples */
/*   for (int i = 0; i < table->height; i++) { */
/*     // Check if the sample is less than the threshold */
/*     if (table->data[feature][i]->data.f < threshold) { */
/*       // Increment the count for the class */
/*       /\* leftClassCounts[]++; *\/ */
/*       totalLeft++; */
/*     } else { */
/*       /\* rightClassCounts[(int)table->target[i]->data.f]++; *\/ */
/*       totalRight++; */
/*     } */
/*   } */
 
/*   return 0.0; */
/* } */

// Split the data
Split *split(Table *table, int feature) {
  // Sort the feature
  Value **thresholds = sortFeature(table, feature);

  // Calculate the averages
  avgFeature(thresholds, table->height);

  return NULL;
}

/* TreeNode *decide(Table *table) { */
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

  /* return NULL; */
/* } */
