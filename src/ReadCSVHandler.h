#ifndef READCSVHANDLER_H
#define READCSVHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"
#include "MTSubTable.h"

class ReadCSVHandler {

public:

ReadCSVHandler(const std::string& filename, off_t chunk_size);

~ReadCSVHandler();




private:
  MMapHandler mmap_hdlr;
  off_t chunk_size;
  void* ptr_location;
  std::string str_data_chunk;


};

#endif
