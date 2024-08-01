#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <Rcpp.h>
#include <string>
#include <sys/types.h>

class FileHandler {

public:
  FileHandler(const std::string& filename);
  ~FileHandler();
  int get_fd() const;
  off_t get_fileSize() const;

private:
  std::string filename;
  int fd;
  off_t fileSize;

};

#endif
