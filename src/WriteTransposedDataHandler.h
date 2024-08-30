#ifndef WRITETRANSPOSEDDATAHANDLER_H
#define WRITETRANSPOSEDDATAHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"

class WriteTransposedDataHandler {
  
public:

WriteTransposedDataHandler(const std::string& filename, off_t size, size_t fragment_size,
                           size_t sync_size, size_t read_size);
  
void write_transpose();
  
private:
  MMapWriteHandler writer;
  //ReadDataHandler source_data;
  TransposeDataHandler source_metadata;
  
  
};

#endif