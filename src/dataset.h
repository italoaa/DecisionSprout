
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
  int id;
} Sample;

typedef struct Target {
  int id;
  int unique;
  char **classes;
} Target;

typedef struct DataSet {
  char *features[MAX_FEATURES];
  int height; // Number of samples
  int width; // Number of features
  int index; // Current index for parsing
  struct Target *target; // Target feature
  struct Sample *tail; // end of the list
  struct Sample *sample; // start of the list
  struct Sample *memory[]; // Memory for the samples
} DataSet;

DataSet *buildDS();

void unique(DataSet *ds, int feature);

void setTarget(DataSet *ds, int target);

void displayDataSet(DataSet *ds, int samples);

void displayHeader(DataSet *ds);

void displaySample(DataSet *ds, Sample *sample);

Sample *addSample(DataSet *ds);

void cleanMem(DataSet *ds);

void removeTail(DataSet *ds);

Sample *getSample(DataSet *ds, int id);

void *insertHeader(DataSet *ds, char *header);

void *insertValue(DataSet *ds, Value value);

// ==================== TABLES ====================

// Subset of the entire dataset
typedef struct Table {
  // Data matrix first index is the feature, second index is the sample
  // The value is a pointer to the float value in the dataset
  Value ***data; 

  char *features[MAX_FEATURES];
  struct Target *target;
  int height; // Number of samples
  int width; // Number of features
} Table;

Table *buildTableFromDS(DataSet *ds);

Table *buildTableFromIds(DataSet *ds, int ids[], int height);

void displayTable(Table *table, int samples);

Table *allocTable();

#endif
