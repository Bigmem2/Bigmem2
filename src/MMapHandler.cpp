#include <Rcpp.h>
#include <sys/mman.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include "FileHandler.h"
#include "MMapHandler.h"

MMapHandler::MMapHandler(const std::string& filename)
  : file(filename), fileData_ptr(nullptr) {

  fileData_ptr = mmap(nullptr, file.get_fileSize(), PROT_READ, MAP_PRIVATE,
                      file.get_fd(), 0);
}

MMapHandler::~MMapHandler() {

  if (fileData_ptr && fileData_ptr != MAP_FAILED) {

    munmap(fileData_ptr, file.get_fileSize());
  }

}


const char* MMapHandler::get_fileData() const {

  return static_cast<const char*>(fileData_ptr);
}

const void* MMapHandler::get_fileData_ptr() const {

  return fileData_ptr;
}

std::string MMapHandler::get_range(off_t start, off_t end) const {

  const char* data_start = static_cast<const char*>(fileData_ptr) + start;
  return std::string(data_start, end - start);
}

FileHandler& MMapHandler::get_file() {

  return file;
}

// read raw data function
// read binary data function

// test examples in cpp
int main() {

  try {

    const std::string file_name = "exdata.csv";

    MMapHandler mmap_handler(file_name);

    // print mmap data and pointer pointer to file
    std::cout << "MMap pointer to file: " << mmap_handler.get_fileData_ptr() << std::endl;
    std::cout << "MMap file contents: " << mmap_handler.get_fileData() << std::endl;

    // check file parameters, get number of bytes
    std::cout << "File descriptor: " << mmap_handler.get_file().get_fd() << std::endl;
    std::cout << "File size: " << mmap_handler.get_file().get_fileSize() << std::endl;

    // map a range into RAM
    std::cout << "Portion of file: " << mmap_handler.get_range(50, 51) << std::endl;


  } catch(const std::exception& e) {

    std::cerr << e.what() << std::endl;
  }
}

// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o MMapHandlerTest src/FileHandler.cpp src/MMapHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR

// run: ./MMapHandlerTest
// remove: rm MMapHandlerTest


