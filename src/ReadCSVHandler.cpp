#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"
#include "MTSubTable.h"
#include "ReadCSVHandler.h"

ReadCSVHandler::ReadCSVHandler(const std::string& filename, off_t chunk_size)
  : mmap_hdlr(filename), chunk_size(chunk_size), chunk_position(chunk_position),
    str_data_chunk(""),  {

  file_size = mmap_hdlr.get_file().get_fileSize();

  ptr_location = static_cast<const char*>(mmap_hdlr.get_fileData_ptr());
}

std::string& next_chunk() {

  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( chunk_position < file_size ) {

    off_t read_length = std::min(chunk_size, file_size - chunk_position);
    const char* chunk_start = ptr_location + chunk_position;
    str_data_chunk = mmap_hdlr.get_range(ptr_location, chunk_start + read_length);

  }

  // update ptr_location and str_data_chunk


  return str_data_chunk&; //?
}


