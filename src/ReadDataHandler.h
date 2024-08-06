#ifndef READCSVHANDLER_H
#define READCSVHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"

class ReadDataHandler {

public:

ReadDataHandler(const std::string& filename, off_t chunk_size);

// ~ReadDataHandler();


std::string& next_chunk();

private:
  MMapHandler mmap_hdlr;
  off_t file_size;
  off_t chunk_size;
  // off_t chunk_position;
  off_t ptr_location;
  const char* ptr_file_start;
  std::string str_data_chunk;


};

#endif
