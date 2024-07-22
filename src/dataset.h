
#ifndef DATASET_H
#define DATASET_H
#define MAX_FEATURES 10

typedef union Data {
  float f;
  char *s;
} Data;

typedef enum DataType {
  FLOAT,
  STRING
} DataType;

typedef struct Value {
  DataType type;
  Data data;
  int sampleID;
} Value;

typedef struct Sample {
  struct Sample *next;
  Value features[MAX_FEATURES]; // NOTE Only takes floats
  int class; // the target is an id of the features
  int id;
} Sample;

typedef struct DataSet {
  char *features[MAX_FEATURES];
  int height; // Number of samples
  int width; // Number of features
  int index; // Current index for parsing
  struct Sample *tail;
  struct Sample *sample;
} DataSet;

DataSet *buildDS();

Value makeValue(DataType type, Data data);

void displayDataSet(DataSet *ds, int samples);

void displayHeader(DataSet *ds);

void displaySample(DataSet *ds, Sample *sample);

Sample *addSample(DataSet *ds);

void *insertHeader(DataSet *ds, char *header);

void *insertValue(DataSet *ds, Value value);

// ==================== TABLES ====================

// Subset of the entire dataset
typedef struct Table {
  // Data matrix first index is the feature, second index is the sample
  // The value is a pointer to the float value in the dataset
  Value ***data; 

  char *features[MAX_FEATURES];
  int height; // Number of samples
  int width; // Number of features
} Table;


Table *buildTable(DataSet *ds);

Table *allocTable();

#endif
