
#ifndef UTILS_H
#define UTILS_H

DataSet *loadCsv(char *filename);


void cb(void *s, size_t len, void *user_data);
void cb2(int c, void *user_data);

#endif