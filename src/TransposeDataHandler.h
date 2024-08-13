#ifndef TRANSPOSEDATAHANDLER_H
#define TRANSPOSEDATAHANDLER_H

#include <Rcpp.h>
#include <string>
#include "ReadDataHandler.h"

class TransposeDataHandler {

public:

TransposeDataHandler(const std::string& filename, off_t chunk_size);

void index_words();

void sort_words();

private:
  ReadDataHandler read_data;
  std::vector<int> word_starts;
  std::vector<int> cumsum_starts;
  const std::string& chunk_of_data;

};


#endif
