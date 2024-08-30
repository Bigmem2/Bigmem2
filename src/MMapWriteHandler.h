#ifndef MMAPHANDLER_H
#define MMAPHANDLER_H

#include <Rcpp.h>
#include <string>
#include <sys/types.h>
#include "FileHandler.h"

class MMapHandler {


public:

MMapHandler(const std::string& filename);

~MMapHandler();


const char* get_fileData() const;

const void* get_fileData_ptr() const;

std::string get_range(off_t start, off_t end) const;

FileHandler& get_file();

// read raw data function
// read binary data function



private:
  FileHandler file;
  void* fileData_ptr;

};




#endif
