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
  ds->tail = NULL;
  ds->features[0] = NULL;
  return ds;
};

void displayDataSet(DataSet *ds, int samples) {
  Sample *sample = ds->sample;
  for (int i = 0; i < samples; i++) {
    displaySample(ds, sample);
    sample = sample->next;
  }
}

void displayHeader(DataSet *ds) {
  printf("[");
  for (int i = 0; i < ds->width; i++) {
    printf("%s, ", ds->features[i]);
  }
  printf("]\n");
}

void displaySample(DataSet *ds, Sample *sample) {
  printf("[");
  for (int i = 0; i < ds->width - 1; i++) {
    printf("%f, ", sample->features[i]);
  }
  printf("%s]\n", sample->class);
}

Sample *addSample(DataSet *ds){
  /* printf("Adding sample\n"); */
  struct Sample *sample = (struct Sample *)malloc(sizeof(struct Sample));

  if (ds->height == 0) {
    ds->sample = sample;
    ds->tail = sample;
  } else {
    ds->tail->next = sample;
    ds->tail = sample;
  }

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

  // Insert value to the last sample
  struct Sample *sample = ds->tail;

  // Check to see if we are reading the target
  if (ds->index == ds->width - 1) {
    sample->class = class;
  } else {
    sample->features[ds->index] = value;
  }
  ds->index++;
  return NULL;
};
