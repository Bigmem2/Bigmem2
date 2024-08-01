#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <Rcpp.h>
#include <string>

class FileHandler {

public:
  FileHandler(const std::string& filename);
  ~FileHandler();
  int get_fd() const;

private:
  int fd;

};

#endif
