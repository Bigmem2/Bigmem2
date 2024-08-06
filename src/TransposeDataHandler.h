#ifndef TRANSPOSEDATAHANDLER_H
#define TRANSPOSEDATAHANDLER_H

#include <Rcpp.h>
#include "ReadDataHandler.h"

class TransposeDataHandler {

public:

TransposeDataHandler(const std::string& filename, off_t chunk_size)

private:
  ReadDataHandler read_data;

};


#endif
