#include <Rcpp.h>
#include <sys/mman.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include "FileWriteHandler.h"
#include "MMapWriteHandler.h"

MMapWriteHandler::MMapWriteHandler(const std::string& filename, off_t size)
  : file(filename, size), fileData_ptr(nullptr) {

  fileData_ptr = mmap(nullptr, file.get_size(), PROT_READ | PROT_WRITE, MAP_SHARED,
                      file.get_fd(), 0);
}

MMapWriteHandler::~MMapWriteHandler() {

  if (fileData_ptr && fileData_ptr != MAP_FAILED) {

    munmap(fileData_ptr, file.get_size());
  }
}


char* MMapWriteHandler::get_fileData() const {

  return static_cast<char*>(fileData_ptr);
}

void* MMapWriteHandler::get_fileData_ptr() const {

  return fileData_ptr;
}

// std::string MMapWriteHandler::get_range(off_t start, off_t end) const {
// 
//   const char* data_start = static_cast<const char*>(fileData_ptr) + start;
//   return std::string(data_start, end - start);
// }

void MMapWriteHandler::write_fragment(size_t offset, const char* chunk, size_t size) {
  
  memcpy(this->get_fileData() + offset, chunk, size);
}

FileWriteHandler& MMapWriteHandler::get_file() {

  return file;
}



// int main() {
//   
//   
//   MMapWriteHandler mmap_write_handler("exdata2.csv", 101);
//   
//   const char* chunk = "Hello, world.";
//   mmap_write_handler.write_fragment(0, chunk, strlen(chunk));
//   
//   mmap_write_handler.write_fragment(0 + strlen(chunk), chunk, strlen(chunk));
//   
// }



// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o MMapWriteHandlerTest src/FileWriteHandler.cpp src/MMapWriteHandler.cpp -L/usr/local/lib/R/lib -lR






// old, for mac os
// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o MMapWriteHandlerTest src/FileHandler.cpp src/MMapWriteHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR

// run: ./MMapWriteHandlerTest
// remove: rm MMapWriteHandlerTest


