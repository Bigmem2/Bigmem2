#ifndef READCSVHANDLER_H
#define READCSVHANDLER_H

#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"

class ReadDataHandler {

public:

ReadDataHandler(const std::string& filename, off_t chunk_size);

// ~ReadDataHandler();


std::string* next_chunk();

std::string* get_chunk(off_t ptr_start);

off_t get_fileSize();

off_t get_ptrLocation();

off_t get_prev_ptrLocation();

off_t get_chunkSize();

void set_chunkSize(off_t size);

bool chunk_in_range();

void reset();

void set_all_chunk_ptrs();

private:
  MMapHandler mmap_hdlr;
  off_t file_size;
  off_t chunk_size;
  // off_t chunk_position;
  off_t ptr_location;
  off_t prev_ptr_location;
  const char* ptr_file_start;
  std::string str_data_chunk;
  std::vector<off_t> all_chunk_ptrs;


};

#endif
