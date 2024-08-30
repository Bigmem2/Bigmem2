#include <Rcpp.h>
#include <fcntl.h>  // For open()
#include <unistd.h> // For close()
#include <string>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>
#include "FileWriteHandler.h"
using namespace Rcpp;


// Constructor that opens the file
FileWriteHandler::FileWriteHandler(const std::string& filename, size_t size)
  : filename(filename), fd(-1), size(size) {

  fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
  if (fd < 0) {
    
      throw std::runtime_error("Error opening file: " + filename);
  }
  
  if (ftruncate(fd, size) == -1) {
    
    throw std::runtime_error("Error allocating memory: " + filename);
  }

  // fileSize = lseek(fd, 0, SEEK_END);
  // if (fileSize == (off_t) -1) {
  //   close(fd);
  //   throw std::runtime_error("Error getting file size: " + filename);
  // }

}

// Destructor that closes the file
FileWriteHandler::~FileWriteHandler() {
  if (fd >= 0) {
    close(fd);
  }
}

  // Method to get the file descriptor
int FileWriteHandler::get_fd() const {
  
  return fd;
}

off_t FileWriteHandler::get_size() const {
  
  return size;
}


// // Example usage
// int main() {
// 
//   try {
// 
//     const std::string file_name = "exdata2.csv";
//     FileWriteHandler file(file_name, 101);
// 
//     // Use file.get_fd() to access the file descriptor
//     int fd = file.get_fd();
//     // Rcout << "File descriptor: " << fd << std::endl;
//     std::cout << "File descriptor: " << fd << std::endl;
// 
//     off_t size = file.get_size();
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




// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o FileWriteHandlerTest src/FileWriteHandler.cpp -L/usr/local/lib/R/lib -lR










// old, not currently working, for macos
// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o FileWriteHandlerTest src/FileWriteHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR
// run: ./FileWriteHandlerTest
// remove: rm FileWriteHandlerTest


