#include <Rcpp.h>
#include <string>
#include "MMapHandler.h"
#include "MTSubTable.h"
#include "ReadCSVHandler.h"

ReadCSVHandler::ReadCSVHandler(const std::string& filename, off_t chunk_size)
  : mmap_hdlr(filename), chunk_size(chunk_size), str_data_chunk("") {

  ptr_location = mmap_hdlr.get_fileData_ptr();
}




