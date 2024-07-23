#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "dataset.h"

// Sort descending
int compare_values_float(const void *a, const void*b) {
  const Value *valueA = (const Value *)a;
  const Value *valueB = (const Value *)b;

  return valueA->data.f - valueB->data.f;
}

// FIXME: Sorting does not work
Value **sortValues(Table *table, int feature) {
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

  // FIXME: does not sort
  qsort(sortedValues, samples, sizeof(Value *), compare_values_float);

  // Print the sorted values
  printf("Sorted values for feature %s\n", table->features[feature]);
  for (int i = 0; i < samples; i++) {
    printf("%f\n", sortedValues[i]->data.f);
  }

  return sortedValues;
}

// Avg between datapoints
// loop over the avgs and make a stump with the avg as threshols
// Calc GINI for each tree and pick the best
