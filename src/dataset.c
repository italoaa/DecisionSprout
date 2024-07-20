// File to handle data reading and parsing
#include <stddef.h> // Ensure this is included
#include <stdlib.h>
#include <stdio.h>
#include "dataset.h"

DataSet *buildDS(){
  DataSet *ds = (DataSet *)malloc(sizeof(DataSet));
  ds->height = 0;
  ds->width = 0;
  ds->sample = NULL;
  ds->features[0] = NULL;
  return ds;
};

void *displaySample(DataSet *ds) {
  if (ds->sample == NULL) {
    // Display the header
    printf("[");
    for (int i = 0; i < ds->width; i++) {
      printf("%s, ", ds->features[i]);
    }
    printf("]\n");
    return NULL;
  }
  printf("[");
  for (int i = 0; i < ds->width - 1; i++) {
    printf("%f, ", ds->sample->features[i]);
  }
  printf("%s]\n", ds->sample->class);
}

Sample *addSample(DataSet *ds){
  /* printf("Adding sample\n"); */
  struct Sample *sample = (struct Sample *)malloc(sizeof(struct Sample));
  sample->next = ds->sample;
  ds->sample = sample;
  ds->height++;
  ds->index = 0;
  return sample;
};

void *insertHeader(DataSet *ds, char *header){
  /* printf("Inserting header: %s\n", header); */
  ds->features[ds->width] = header;
  ds->width++;
  return NULL;
};

void *insertValue(DataSet *ds, float value, char *class){
  // Class represents the value in string form
  // we later use the index to determine
  // if we are reading the target
  /* printf("Inserting value: %s\n", class); */

  struct Sample *sample = ds->sample;

  // Check to see if we are reading the target
  if (ds->index == ds->width - 1) {
    sample->class = class;
  } else {
    sample->features[ds->index] = value;
  }
  ds->index++;
  return NULL;
};
