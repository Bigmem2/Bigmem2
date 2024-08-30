#ifndef FILEWRITEHANDLER_H
#define FILEWRITEHANDLER_H

#include <Rcpp.h>
#include <string>
#include <sys/types.h>

class FileWriteHandler {

public:
  FileWriteHandler(const std::string& filename, size_t size);
  ~FileWriteHandler();
  int get_fd() const;
  off_t get_size() const;

private:
  std::string filename;
  int fd;
  off_t size;

};

#endif
