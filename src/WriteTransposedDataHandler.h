#ifndef WRITETRANSPOSEDDATAHANDLER_H
#define WRITETRANSPOSEDDATAHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"

class WriteTransposedDataHandler {
  
public:

WriteTransposedDataHandler(const std::string& filename, off_t size, size_t fragment_size,
                           size_t sync_size);
  
void write_transpose();
  
private:
  MMapWriteHandler writer;
  
  
  
};

#endif