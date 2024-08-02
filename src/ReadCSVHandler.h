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


std::string& next_chunk();

private:
  MMapHandler mmap_hdlr;
  off_t file_size;
  off_t chunk_size;
  off_t chunk_position;
  const char* ptr_location;
  std::string str_data_chunk;


};

#endif
