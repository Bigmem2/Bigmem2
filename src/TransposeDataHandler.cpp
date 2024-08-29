#include <Rcpp.h>
#include <chrono> // for testing the timing
#include "TransposeDataHandler.h"

TransposeDataHandler::TransposeDataHandler(const std::string& filename, off_t chunk_size)
  : read_data(filename, chunk_size), word_starts({0}), cumsum_starts({0}) {

  this->chunk_size = chunk_size;
  this->n_row = this->ncol();
  this->n_col = this->nrow();

}


int TransposeDataHandler::ncol() {
  
  n_col = 1;
  
  std::string* chunkPtr;
  
  bool inFirstRow = true;
  
  while( inFirstRow && (chunkPtr = read_data.next_chunk()) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if( inFirstRow && element == ',' ) {
        
        n_col += 1;
        
      } else if( element == '\n' ) {
        
        inFirstRow = false;
        
        break;
      }
    }
    
  }
  
  read_data.reset();
  
  std::cout << n_col << std::endl;
  
  return n_col;

}

int TransposeDataHandler::nrow() {
  
  n_row = 0;
  
  std::string* chunkPtr;

  while( ( chunkPtr = read_data.next_chunk()) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if(element == '\n') {
        
        n_row++;
      }
    }
  }
  
  std::string* last_chunk = read_data.get_chunk(read_data.get_prev_ptrLocation());
  
  if( (*last_chunk)[last_chunk->size() - 1] != '\n' ) {

    n_row += 1;
  }
  
  read_data.reset();
  
  std::cout << n_row << std::endl;
  
  return n_row;

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
    
    auto start = std::chrono::high_resolution_clock::now();
//3650722816/50
    TransposeDataHandler data("../big_data.csv", 100*2^20);
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
    std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";
    
  } catch(const std::exception& e) {
    
    std::cerr << e.what() << std::endl;
  }
}








// new, working
// on linux rstudio server container:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o TransponseDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/usr/local/lib/R/lib -lR


















// old, not working
// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o TransposeDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR






