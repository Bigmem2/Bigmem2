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
FileHandler::FileHandler(const std::string& filename)
  : filename(filename), fd(-1) {

  fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
      throw std::runtime_error("Error opening file: " + filename);
  }

  fileSize = lseek(fd, 0, SEEK_END);
  if (fileSize == (off_t) -1) {
    close(fd);
    throw std::runtime_error("Error getting file size: " + filename);
  }

}

// Destructor that closes the file
FileHandler::~FileHandler() {
  if (fd >= 0) {
    close(fd);
  }
}

  // Method to get the file descriptor
int FileHandler::get_fd() const {
  return fd;
}

off_t FileHandler::get_fileSize() const {
  return fileSize;
}


// // Example usage
// int main() {
//
//   try {
//
//     const std::string file_name = "exdata.csv";
//     FileHandler file(file_name);
//
//     // Use file.get_fd() to access the file descriptor
//     int fd = file.get_fd();
//     // Rcout << "File descriptor: " << fd << std::endl;
//     std::cout << "File descriptor: " << fd << std::endl;
//
//     off_t size = file.get_fileSize();
//     // Rcout << "File size: " << size << std::endl;
//     std::cout << "File size: " << size << std::endl;
//
//
//     // Perform file operations here
//
//   } catch (const std::exception& e) {
//
//     std::cerr << e.what() << std::endl;
//   }
//
//   return 0;
// }



// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o FileHandlerTest src/FileHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR
// run: ./FileHandlerTest
// remove: rm FileHandlerTest






