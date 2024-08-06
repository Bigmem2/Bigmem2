#include <Rcpp.h>
#include <string>
#include <chrono> // for testing the timing
#include "MMapHandler.h"
#include "ReadDataHandler.h"

ReadDataHandler::ReadDataHandler(const std::string& filename, off_t chunk_size)
  : mmap_hdlr(filename), chunk_size(chunk_size), str_data_chunk("")  {

  file_size = mmap_hdlr.get_file().get_fileSize();

  ptr_file_start = static_cast<const char*>(mmap_hdlr.get_fileData_ptr());
  ptr_location = 0;
}

std::string& ReadDataHandler::next_chunk() {

  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( ptr_location < file_size ) {

    off_t read_length = std::min(chunk_size, file_size - ptr_location);

    // const char* chunk_start = ptr_file_start + ptr_location;
    str_data_chunk = mmap_hdlr.get_range(ptr_location, ptr_location + read_length);

    ptr_location += read_length;

  } else {

    str_data_chunk = "";
  }

  // update ptr_location and str_data_chunk
  // do we need to update ptr_location
  // i think that chunk_position and ptr_location do the same thing. Is that true?
  // if redundent, eliminate one?
  // chat gpt agrees - update the file

  return str_data_chunk;
}


// test
int main() {

  try{
    // instantiate a new read data handler
    // off_t bytes = static_cast<off_t>(10);

    auto start = std::chrono::high_resolution_clock::now();

    ReadDataHandler read_data("flights.csv", 16000*9);

    // std::cout << "Next chunk: " << read_data.next_chunk() << std::endl;

    std::string stop = "1";
    int ctr;
    ctr = 1;
    while(stop != "") {
      // std::cout << stop << ctr << std::endl;

      stop = read_data.next_chunk();
      ctr += 1;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
    std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";

    // read some chunks to test the main method, next_chunk()

  } catch(const std::exception& e) {

    std::cerr << e.what() << std::endl;
  }


}

// compile and run

// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o ReadDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR

// run: ./ReadDataHandlerTest
// remove: rm ReadDataHandlerTest




