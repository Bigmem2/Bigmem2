#include <Rcpp.h>
#include <chrono> // for testing the timing
#include "TransposeDataHandler.h"

TransposeDataHandler::TransposeDataHandler(const std::string& filename, off_t chunk_size)
  : read_data(filename, chunk_size), word_starts({0}), cumsum_starts({0}) {

  this->chunk_size = chunk_size;
}


void TransposeDataHandler::ncol() {
  
  std::string* chunkPtr;
  
  int ncol = 0;
  
  bool inFirstRow = true;
  
  while( inFirstRow && (chunkPtr = read_data.next_chunk()) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if( inFirstRow && element == ',' ) {
        
        ncol += 1;
        
      } else if( element == '\n' ) {
        
        inFirstRow = false;
      }
    }
    
  }
  
  std::cout << ncol << std::endl;

}

void TransposeDataHandler::nrow() {
  
  std::string* chunkPtr;
  
  int nrow = 0;
  
  off_t ptr_location;

  while( (chunkPtr = read_data.next_chunk()) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if(element == '\n') {
        
        nrow += 1;
      }
    }
    
    ptr_location = read_data.get_ptrLocation();
  }
  
  // std::cout << ptr_location << std::endl;
  
  std::string* last_chunk = read_data.get_chunk(ptr_location - read_data.get_chunkSize());
  
  // std::cout << *last_chunk << std::endl;
  
  if( (*last_chunk)[last_chunk->size() - 1] == '\n' ) {

    nrow -= 1;
  }
  
  std::cout << nrow << std::endl;
}

// void TransposeDataHandler::push_wordlengths() {
//   
//   
//   
//   while( inFirstRow && (chunkPtr = read_data.next_chunk()) != nullptr ) {
//     
//     for(size_t i = 0; i < chunkPtr->size(); ++i) {
//       
//       char element = (*chunkPtr)[i];
//       
//       if( inFirstRow && element == ',' ) {
//         
//         ncol += 1;
//         
//       } else if( element == '\n' ) {
//         
//         inFirstRow = false;
//       }
//     }
//     
//   }
//   
// }

int main() {
  try {
    
    TransposeDataHandler data("flights.csv", 1600);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    data.ncol();
    
    data.nrow();
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
    std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";
    
  } catch(const std::exception& e) {
    
    std::cerr << e.what() << std::endl;
  }
}







// old, not working
// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o TransposeDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR


// new, working
// on linux rstudio server container:
// clang++ -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o TransponseDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/usr/local/lib/R/lib -lR








