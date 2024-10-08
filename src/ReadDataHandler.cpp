#include <Rcpp.h>
#include <string>
#include <chrono> // for testing the timing
#include "MMapHandler.h"
#include "ReadDataHandler.h"

thread_local std::string ReadDataHandler::str_data_chunk = "";

ReadDataHandler::ReadDataHandler(const std::string& filename, off_t chunk_size)
  : mmap_hdlr(filename), chunk_size(chunk_size)  {

  file_size = mmap_hdlr.get_file().get_fileSize();

  ptr_file_start = static_cast<const char*>(mmap_hdlr.get_fileData_ptr());
  ptr_location = 0;
  
  set_all_chunk_ptrs();
}

std::string* ReadDataHandler::next_chunk() {

  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( ptr_location < file_size ) {

    off_t read_length = std::min(chunk_size, file_size - ptr_location);

    // const char* chunk_start = ptr_file_start + ptr_location;
    str_data_chunk = mmap_hdlr.get_range(ptr_location, ptr_location + read_length);

    prev_ptr_location = ptr_location;
    
    ptr_location += read_length;
    
    return &str_data_chunk;

  } else {

    return nullptr;
  }
}

std::string* ReadDataHandler::get_chunk(off_t ptr_start) {
  
  str_data_chunk.clear();
  
  // use ptr_location and mmap and chunk_size to fill str_data_chunk with new data
  if( ptr_start < file_size ) {
    
    off_t read_length = std::min(chunk_size, file_size - ptr_start);
    
    // const char* chunk_start = ptr_file_start + ptr_location;
    str_data_chunk = mmap_hdlr.get_range(ptr_start, ptr_start + read_length);
    
    ptr_start += read_length; // this looks redundant! review and remove
    
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

off_t ReadDataHandler::get_prev_ptrLocation() {
  
  return prev_ptr_location;
}

off_t ReadDataHandler::get_chunkSize() {
  
  return chunk_size;
}

bool ReadDataHandler::chunk_in_range() {

  return ptr_location < file_size;
}

void ReadDataHandler::reset() {
  
  this->ptr_location = 0;
}

void ReadDataHandler::set_chunkSize(off_t size) {
  
  this->chunk_size = size;
}

void ReadDataHandler::set_all_chunk_ptrs() {
  
  off_t current_ptr = 0;
  
  while ( current_ptr < file_size ) {
    
    this->all_chunk_ptrs.push_back( current_ptr );
    
    current_ptr += chunk_size;
  }
}

const std::vector<off_t>& ReadDataHandler::get_all_chunk_ptrs() const {
  
  return all_chunk_ptrs;
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
//     ReadDataHandler read_data("flights.csv", 500000);
// 
//     // std::cout << "Next chunk: " << read_data.next_chunk() << std::endl;
// 
//     std::vector<size_t> new_line_pos;
// 
//     std::cout << "File size: " << read_data.get_fileSize() << std::endl;
//     
//     // std::cout << "All chunk ptrs: " << read_data.get_all_chunk_ptrs() << std::endl;
//     
//     std::cout << "All chunk ptrs: [";
//     const std::vector<off_t>& chunk_ptrs = read_data.get_all_chunk_ptrs();
//     for (size_t i = 0; i < chunk_ptrs.size(); ++i) {
//       std::cout << chunk_ptrs[i];
//       if (i != chunk_ptrs.size() - 1) {
//         std::cout << ", "; // Add comma between elements
//       }
//     }
//     std::cout << "]" << std::endl;
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




