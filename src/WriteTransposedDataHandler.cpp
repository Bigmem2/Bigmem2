#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"
#include "WriteTransposedDataHandler.h"

WriteTransposedDataHandler::WriteTransposedDataHandler(const std::string& filename, 
  const std::string& filename2, off_t size, size_t sync_threshold, size_t read_size)
  : writer(filename2, size, sync_threshold), data(filename, read_size) {
  
  write_transpose();
}

void WriteTransposedDataHandler::write_transpose() {
  
  ReadDataHandler* read_data = data.get_ReadDataHandler();
  
  std::string* chunkPtr;
  int di = 0;
  int dj = 0;
  
  while( ( chunkPtr = read_data->next_chunk()) != nullptr ) {
    
    std::string word;
    int word_len = 0;
    int frag_len = 0;

    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if( element == ','  || element == '\n' ) {
        
        continue;
      }
      
      word.push_back( element );
      word_len += 1;
      
      frag_len = data.get_elem_wordTable(di, dj);
      
      if( word_len >=  frag_len - 1 ) {
        
        if( dj < data.get_n_row() - 1 ) { 
          
          word.push_back( ',' );
        } else {
          
          word.push_back( '\n' );
        }

        writer.write_fragment( data.get_elem_cumWordTable(di, dj) - frag_len, word.c_str(), frag_len );
        word.clear();
        
        di += 1;
        
        if( di == data.get_n_col() ) {
          
          di = 0;
          dj += 1;
        }
        
        word_len = 0;
      }
    }
  }
}

int main() {
  
  // WriteTransposedDataHandler transpose_data("output.csv", "output_transpose.csv", 20708, 2*100*2^20, 100*2^20);
  
  WriteTransposedDataHandler transpose_data("flights.csv", "flights_transpose.csv", 33406100, 33406100, 33406100);
  
  // left off with this working. the chunk writing does not seem to work! Which part is breaking???
  
  // transpose_data.write_transpose();
  
}

// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o WriteTransposedDataHandlerTest src/FileHandler.cpp src/FileWriteHandler.cpp src/MMapHandler.cpp src/MMapWriteHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp src/WriteTransposedDataHandler.cpp -L/usr/local/lib/R/lib -lR














