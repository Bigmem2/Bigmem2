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


const char* get_fileData_ptr() const;

// read raw data function
// read binary data function



private:
  FileHandler file;
  void* fileData_ptr;

};




#endif
