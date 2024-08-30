#ifndef MMAPWRITEHANDLER_H
#define MMAPWRITEHANDLER_H

#include <Rcpp.h>
#include <string>
#include <sys/types.h>
#include "FileHandler.h"

class MMapWriteHandler {


public:

MMapWriteHandler(const std::string& filename, off_t size);

~MMapWriteHandler();


char* get_fileData() const;

void* get_fileData_ptr() const;

// std::string get_range(off_t start, off_t end) const;

void write_fragment(size_t offset, const char* chunk, size_t size);

FileWriteHandler& get_file();

// read raw data function
// read binary data function



private:
  FileWriteHandler file;
  void* fileData_ptr;

};




#endif
