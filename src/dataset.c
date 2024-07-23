// File to handle data reading and parsing
#include <stddef.h> // Ensure this is included
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

void unique(DataSet *ds, int feature) {
  // Find if its a string or float
  int type = STRING;
  if (ds->sample->features[feature].type == FLOAT) {
    type = FLOAT;
  }

  // Loop over the feature and count the unique values
  Target *target = ds->target;
  Sample *sample = ds->sample;

  // BEWARE IT IS EXTREMELY INEFFICIENT
  // dont die
  // later we can optimize this
  for (int i = 0; i < ds->height; i++) {
    int found = 0;
    for (int j = 0; j < i; j++) {
      if (type == STRING) {
	char *a = sample->features[feature].data.s;
	char *b = getSample(ds, j)->features[feature].data.s;
	// if either is null break
	if (a == NULL || b == NULL) {
	  found = 1;
	  /* printf("Null\n"); */
	  break;
	}
	if (strcmp(a, b) == 0) {
	  found = 1;
	  break;
	}
      } else {
	printf("Float\n");
	// not implementing yet
	break;
      }
    }
    if (found == 0) {
      // Add class to the target
      if (type == STRING) {
	target->classes[target->unique] = sample->features[feature].data.s;
	target->unique++;
      }
    }
    if (sample->next == NULL) {
      break;
    }
    sample = sample->next;
  }
}

void setTarget(DataSet *ds, int target) {
  if (target >= ds->width || target < 0) {
    // Error
    printf("Error: Target out of bounds\n");
    return;
  }
  Target *t = (Target *)malloc(sizeof(Target));
  t->unique = 0;
  // allocate memory for the classes
  t->classes = (char **)malloc(ds->height * sizeof(char *));
  t->id = target;
  ds->target = t;

  unique(ds, 5); // find unique values
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

void removeTail(DataSet *ds) {
  // Remove the tail
  Sample *new_tail = getSample(ds, ds->height - 2);
  Sample *tail = new_tail->next;
  // Free from the memory
  ds->memory[ds->height - 1] = NULL;
  free(tail);
  new_tail->next = NULL;
  ds->tail = new_tail;
  ds->height--;
}

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
  table->target = ds->target;

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
  table->target = ds->target;

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

void encode_Labels(Table *table, int id) {
  // Get the labels
  Value **labels = table->data[id];
  // loop over the labels
  for (int i = 0; i < table->height; i++) {
    Value *label = labels[i];
    for (int j = 0; j < table->target->unique; j++) {
      if (strcmp(label->data.s, table->target->classes[j]) == 0) {
	label->data.f = j;
	break;
      }
    }
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
