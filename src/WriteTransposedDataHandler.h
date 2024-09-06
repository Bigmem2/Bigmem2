#ifndef WRITETRANSPOSEDDATAHANDLER_H
#define WRITETRANSPOSEDDATAHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"

class WriteTransposedDataHandler {
  
public:

WriteTransposedDataHandler(const std::string& filename, const std::string& filename2, 
                           off_t sync_threshold, off_t read_size, int num_threads);
  
void write_transpose();
void write_transpose_multithread();
  
private:
  TransposeDataHandler data;
  MMapWriteHandler writer;
  //ReadDataHandler source_data;
  int num_threads;
  
};

#endif