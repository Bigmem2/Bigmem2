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

private:
  int fd;
  off_t fileSize;

};

#endif
