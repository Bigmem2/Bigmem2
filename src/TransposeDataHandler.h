#ifndef TRANSPOSEDATAHANDLER_H
#define TRANSPOSEDATAHANDLER_H

#include <Rcpp.h>
#include <string>
#include "ReadDataHandler.h"

class TransposeDataHandler {

public:

TransposeDataHandler(const std::string& filename, off_t chunk_size);

int ncol();
int nrow();

void sort_words();

private:
  //std::string& filename;
  off_t chunk_size;
  ReadDataHandler read_data;
  std::vector<int> word_starts;
  std::vector<int> cumsum_starts;
  int n_col;
  int n_row;
  // const std::string& chunk_of_data;

};


#endif
