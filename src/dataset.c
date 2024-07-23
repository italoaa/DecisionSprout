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
  ds->memory[0] = NULL;
  return ds;
};

void displayDataSet(DataSet *ds, int samples) {
  if (samples > ds->height) {
    // Error
    printf("Error: Not enough samples\n");
    return;
  }
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
  printf("%d [", sample->id);
  for (int i = 0; i < ds->width; i++) {
    Value value = sample->features[i];
    if (value.type == FLOAT) {
      printf("%f, ", value.data.f);
    } else {
      printf("%s, ", value.data.s);
    }
  }
  printf("]\n");
}

void cleanMem(DataSet *ds) {
  for (int i = 0; i < ds->height; i++) {
    // Set to null
    ds->memory[i] = NULL;
  }
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

  sample->id = ds->height;
  ds->height++;
  ds->index = 0;
  return sample;
};

Sample *getSample(DataSet *ds, int id) {
  // Check if the sample is in memory
  if (ds->memory[id] != NULL) {
    // Return the sample from memory if it exists
    /* printf("Returning from memory id: %d\n", id); */
    return ds->memory[id];
  }
  Sample *sample = ds->sample;
  for (int i = 0; i < id; i++) {
    sample = sample->next;

  }
  /* printf("Returning from sample, saving id %d to mem\n", id); */
  // Save the sample in memory
  ds->memory[id] = sample;
  return sample;
}

void *insertHeader(DataSet *ds, char *header){
  /* printf("Inserting header: %s\n", header); */
  ds->features[ds->width] = header;
  ds->width++;
  return NULL;
};

void *insertValue(DataSet *ds, Value value){
  // Class represents the value in string form
  // we later use the index to determine
  // if we are reading the target
  /* printf("Inserting value: %s\n", class); */

  // Insert value to the last sample
  struct Sample *sample = ds->tail;

  // print the sample id
  value.sampleID = sample->id;
  sample->features[ds->index] = value;
  ds->index++;
  return NULL;
};

// ==================== TABLES ====================

// Tables are implementations of a dataset with a matrix for faster access
// ordered by columns instead of rows
Table *buildTableFromDS(DataSet *ds){
  Table *table = allocTable();

  table->height = ds->height;
  table->width = ds->width; // includes the target

  // Allocate memory for the data matrix
  // NOTE: could be improved with contiguous memory allocation
  table->data = (Value ***)malloc(table->width * sizeof(Value **));

  // Width
  for (int i = 0; i < table->width; i++) {
    // Height of the table
    table->data[i] = (Value **)malloc(table->height * sizeof(Value *));
  }

  // Populate the data matrix
  Sample *sample = ds->sample;
  for (int i = 0; i < table->height; i++) {
    for (int j = 0; j < table->width; j++) {
      table->data[j][i] = &sample->features[j];
    }
    sample = sample->next;
  }
  
  // Copy the features
  for (int i = 0; i < table->width; i++) {
    table->features[i] = ds->features[i];
  }

  return table;
};

// Ids is the full list of ids for the table
// Height is the number of ids
Table *buildTableFromIds(DataSet *ds, int ids[], int height) {
  Table *table = allocTable();

  table->height = height;
  table->width = ds->width; // includes the target

  // Allocate memory for the data matrix
  // NOTE: could be improved with contiguous memory allocation
  table->data = (Value ***)malloc(table->width * sizeof(Value **));

  // Width
  for (int i = 0; i < table->width; i++) {
    // Height of the table
    table->data[i] = (Value **)malloc(table->height * sizeof(Value *));
  }

  // Populate the data matrix
  for (int i = 0; i < height; i++) {
    Sample *sample = getSample(ds, ids[i]);
    for (int j = 0; j < table->width; j++) {
      table->data[j][i] = &sample->features[j];
    }
  }
	
  // Copy the features
  for (int i = 0; i < table->width; i++) {
    table->features[i] = ds->features[i];
  }

  return table;
} 

void displayTable(Table *table, int samples) {
  if (samples > table->height) {
    // Error
    printf("Error: Not enough samples\n");
    return;
  }
  for (int i = 0; i < samples; i++) {
    for (int j = 0; j < table->width; j++) {
      Value *value = table->data[j][i];
      if (value->type == FLOAT) {
	printf("%f, ", value->data.f);
      } else {
	printf("%s, ", value->data.s);
      }
    }
    printf("\n");
  }
}

Table *allocTable() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->height = 0;
  table->width = 0;
  table->data = NULL;
  table->features[0] = NULL;
  return table;
}
