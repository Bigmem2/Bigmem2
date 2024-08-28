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

std::string* ReadDataHandler::next_chunk() {

  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( ptr_location < file_size ) {

    off_t read_length = std::min(chunk_size, file_size - ptr_location);

    // const char* chunk_start = ptr_file_start + ptr_location;
    str_data_chunk = mmap_hdlr.get_range(ptr_location, ptr_location + read_length);

    ptr_location += read_length;

    return &str_data_chunk;

  } else {

    return nullptr;
  }
}

std::string* ReadDataHandler::get_chunk(off_t ptr_start) {
  
  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( ptr_start < file_size ) {
    
    off_t read_length = std::min(chunk_size, file_size - ptr_start);
    
    // const char* chunk_start = ptr_file_start + ptr_location;
    str_data_chunk = mmap_hdlr.get_range(ptr_start, ptr_start + read_length);
    
    ptr_start += read_length;
    
    return &str_data_chunk;
    
  } else {
    
    return nullptr;
  }
}

off_t ReadDataHandler::get_fileSize() {

  return file_size;
}

off_t ReadDataHandler::get_ptrLocation() {

  return ptr_location;
}

off_t ReadDataHandler::get_chunkSize() {
  
  return chunk_size;
}

bool ReadDataHandler::chunk_in_range() {

  return ptr_location < file_size;
}

// test
// int main() {
// 
//   try{
//     // instantiate a new read data handler
//     // off_t bytes = static_cast<off_t>(10);
// 
//     auto start = std::chrono::high_resolution_clock::now();
// 
//     ReadDataHandler read_data("flights.csv", 1600);
// 
//     // std::cout << "Next chunk: " << read_data.next_chunk() << std::endl;
// 
//     int ctr;
//     ctr = 1;
//     std::vector<size_t> new_line_pos;
//     off_t starts_here;
// 
//     std::cout << "File size: " << read_data.get_fileSize() << std::endl;
//     
//     std::string* data;
// 
//     while( (data = read_data.next_chunk()) != nullptr ) {
//       // std::cout << *data << std::endl;
//       
//       // std::cout << ctr << std::endl;
//       // starts_here = read_data.get_ptrLocation();
//       // 
//       // stop = read_data.next_chunk();
//       // 
//       // // search for metadata for the transformation step (next method)
//       // for (size_t i = 0; i < stop.size(); ++i) {
//       //   if (stop[i] == '\n') {
//       //     new_line_pos.push_back( starts_here + i ); // could we do this in a memory mapped way instead?? just a thought
//       //   }
//       // }
//       // 
//       // ctr += 1;
//     }
// 
//     auto end = std::chrono::high_resolution_clock::now();
// 
//     auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
//     std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
//     std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";
// 
// 
//     std::cout << "All new line positions detected: ";
//     for (size_t pos : new_line_pos) {
//       std::cout << pos << " ";
//     }
//     std::cout << "\n";
// 
// 
//     // read some chunks to test the main method, next_chunk()
// 
//   } catch(const std::exception& e) {
// 
//     std::cerr << e.what() << std::endl;
//   }
// 
// 
// }













// // old test
// int main() {
//
//   try{
//     // instantiate a new read data handler
//     // off_t bytes = static_cast<off_t>(10);
//
//     auto start = std::chrono::high_resolution_clock::now();
//
//     ReadDataHandler read_data("transposeit.csv", 160);
//
//     // std::cout << "Next chunk: " << read_data.next_chunk() << std::endl;
//
//     std::string stop = "1";
//     int ctr;
//     ctr = 1;
//     std::vector<size_t> new_line_pos;
//     off_t starts_here;
//
//     std::cout << "File size: " << read_data.get_fileSize() << std::endl;
//
//     while( read_data.chunk_in_range() ) {
//       // std::cout << ctr << std::endl;
//
//
//       starts_here = read_data.get_ptrLocation();
//
//       stop = read_data.next_chunk();
//
//       // search for metadata for the transformation step (next method)
//       for (size_t i = 0; i < stop.size(); ++i) {
//         if (stop[i] == '\n') {
//           new_line_pos.push_back( starts_here + i ); // could we do this in a memory mapped way instead?? just a thought
//         }
//       }
//
//       ctr += 1;
//     }
//
//     auto end = std::chrono::high_resolution_clock::now();
//
//     auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
//     std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
//     std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";
//
//
//     std::cout << "All new line positions detected: ";
//     for (size_t pos : new_line_pos) {
//       std::cout << pos << " ";
//     }
//     std::cout << "\n";
//
//
//     // read some chunks to test the main method, next_chunk()
//
//   } catch(const std::exception& e) {
//
//     std::cerr << e.what() << std::endl;
//   }
//
//
// }

// compile and run

// compile on the fly for testing just this component
// on macos:
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o ReadDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR

// on linux rstudio server container:
// clang++ -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o ReadDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp -L/usr/local/lib/R/lib -lR

// run: ./ReadDataHandlerTest
// remove: rm ReadDataHandlerTest




