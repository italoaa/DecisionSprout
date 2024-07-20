
#ifndef DATASET_H
#define DATASET_H
#define MAX_FEATURES 10

typedef struct Sample {
  struct Sample *next;
  float features[MAX_FEATURES]; // Only takes floats
  char *class;
} Sample;

typedef struct DataSet {
  char *features[MAX_FEATURES];
  int height; // Number of samples
  int width; // Number of features
  int index; // Current index for parsing
  struct Sample *sample;
} DataSet;

DataSet *buildDS();

void *displaySample(DataSet *ds);

Sample *addSample(DataSet *ds);

void *insertHeader(DataSet *ds, char *header);

void *insertValue(DataSet *ds, float value, char* class);

#endif
