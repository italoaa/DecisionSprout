// Utilities to make my life easier
#include <stdlib.h>
#include <csv.h>
#include <stdlib.h>
#include <stddef.h> // Ensure this is included
#include <string.h>

#include "dataset.h"
#include "tree.h"

DataSet *ds;

// call back for each value read
void cb(void *s, size_t len, void *user_data) {
  // If the dataset has no samples insert a header
  (void)user_data; // Suppress unused parameter warning
  char *str = (char *)malloc(len + 1);
  strncpy(str, s, len);
  str[len] = '\0'; // Null-terminate the string
  if (ds->height == 0) {
    insertHeader(ds, str);
  }
  else {
    Data data;
    DataType type = FLOAT;
    // Otherwise insert a value
    // NOTE: Let the user define this
    if (ds->index == ds->width - 1) {
      // Its the last value, so its the class
      data.s = str;
      type = STRING;
    } else {
      data.f = atof(str);
      free(str);
    }
    Value value;
    value.type = type;
    value.data = data;
    value.sampleID = -1;
    insertValue(ds, value);
  }
}

// Call back at the end of the sample
void cb2(int c, void *user_data) {
  /* printf("End of record\n"); */
  (void)user_data; // Suppress unused parameter warning
  (void)c;
  // Create a new sample
  addSample(ds);
}

// Load a csv from a file
DataSet *loadCsv(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Failed to open file");
    return NULL;
  }

  struct csv_parser p;
  csv_init(&p, 0);

  ds = buildDS();

  char buf[1024];
  size_t bytes_read;
  while ((bytes_read = fread(buf, 1, sizeof(buf), fp)) > 0) {
    if (csv_parse(&p, buf, bytes_read, cb, cb2, NULL) != bytes_read) {
      fprintf(stderr, "Error: %s\n", csv_strerror(csv_error(&p)));
      return NULL;
    }
  }

  csv_fini(&p, cb, cb2, NULL);
  csv_free(&p);
  fclose(fp);

  setUpMem(ds);
  removeTail(ds);

  return ds;
}

// VISUALIZATION

void generate_dot(TreeNode *node, FILE *file) {
    static int null_count = 0;

    if (node->split != NULL) {
        fprintf(file, "  node%p [label=\"Feature %d\\nThreshold %.2f\\nGini %.2f\"];\n",
                (void *)node, node->split->feature, node->split->threshold, node->split->gini);
    } else {
        fprintf(file, "  node%p [label=\"Leaf\"];\n", (void *)node);
    }

    if (node->left) {
        fprintf(file, "  node%p -> node%p [label=\"True\"];\n", (void *)node, (void *)node->left);
        generate_dot(node->left, file);
    } else {
        fprintf(file, "  null%d [shape=point];\n", null_count);
        fprintf(file, "  node%p -> null%d [label=\"True\"];\n", (void *)node, null_count);
        null_count++;
    }

    if (node->right) {
        fprintf(file, "  node%p -> node%p [label=\"False\"];\n", (void *)node, (void *)node->right);
        generate_dot(node->right, file);
    } else {
        fprintf(file, "  null%d [shape=point];\n", null_count);
        fprintf(file, "  node%p -> null%d [label=\"False\"];\n", (void *)node, null_count);
        null_count++;
    }
}

void export_tree_to_dot(TreeNode *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fprintf(file, "digraph Tree {\n");
    fprintf(file, "  node [shape=box];\n");

    if (root) {
        generate_dot(root, file);
    } else {
        fprintf(file, "\n");
    }

    fprintf(file, "}\n");
    fclose(file);
}
