#include <Rcpp.h>
#include <fcntl.h>  // For open()
#include <unistd.h> // For close()
#include <string>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>
#include "FileHandler.h"
using namespace Rcpp;


// Constructor that opens the file
FileHandler(const std::string& filename) {
  fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
      throw std::runtime_error("Error opening file: " + filename);
  }

  fileSize = lseek(fd, 0, SEEK_END);

}

// Destructor that closes the file
~FileHandler() {
  if (fd >= 0) {
    close(fd);
  }
}

  // Method to get the file descriptor
int get_fd() const {
  return fd;
}

off_t get_fileSize() const {
  return fileSize;
}


// Example usage
int main() {

  try {

    FileHandler file("exdata.csv");

    // Use file.get_fd() to access the file descriptor
    int fd = file.get_fd();
    Rcout << "File descriptor: " << fd << std::endl;

    off_t size = file.get_fileSize();
    Rcout << "File size: " << size << std::endl;


    // Perform file operations here

  } catch (const std::exception& e) {

    std::cerr << e.what() << std::endl;
  }

  return 0;
}



// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG  -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp    -I/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include -fPIC  -falign-functions=64 -Wall -g -O2 -o FileHandlerTest src/FileHandler.cpp
// run with: ./FileHandlerTest
// remove: rm FileHandlerTest





