#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"
#include "ReadDataHandler.h"

ReadDataHandler::ReadDataHandler(const std::string& filename, off_t chunk_size)
  : mmap_hdlr(filename), chunk_size(chunk_size), chunk_position(chunk_position),
    str_data_chunk(""),  {

  file_size = mmap_hdlr.get_file().get_fileSize();

  ptr_location = static_cast<const char*>(mmap_hdlr.get_fileData_ptr());
}

std::string& ReadDataHandler::next_chunk() {

  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( chunk_position < file_size ) {

    off_t read_length = std::min(chunk_size, file_size - chunk_position);
    const char* chunk_start = ptr_location + chunk_position;
    str_data_chunk = mmap_hdlr.get_range(chunk_start, chunk_start + read_length);

    chunk_position += read_length;
  }

  // update ptr_location and str_data_chunk
  // do we need to update ptr_location
  // i think that chunk_position and ptr_location do the same thing. Is that true?
  // if redundent, eliminate one?
  // chat gpt agrees - update the file

  return str_data_chunk;
}


