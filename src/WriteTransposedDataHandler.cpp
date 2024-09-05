#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"
#include "WriteTransposedDataHandler.h"


//: writer(filename2, size, sync_threshold), data(filename, read_size) 

WriteTransposedDataHandler::WriteTransposedDataHandler(const std::string& filename, 
  const std::string& filename2, off_t sync_threshold, off_t read_size)
  : data(filename, read_size),
    writer(filename2, data.get_ReadDataHandler()->get_fileSize(), sync_threshold) {
  
  write_transpose();
}

void WriteTransposedDataHandler::write_transpose() {
  
  ReadDataHandler* read_data = data.get_ReadDataHandler();
  
  int n_row = data.get_n_row();
  int n_col = data.get_n_col();
  
  std::string* chunkPtr;
  int di = 0;
  int dj = 0;
  
  std::string word;
  word.reserve( 1024 );
  int word_len = 0;
  
  while( ( chunkPtr = read_data->next_chunk()) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if( element == ','  || element == '\n' ) {
        
        continue;
      }
      
      word.push_back( element );
      ++word_len;
      
      int frag_len = data.get_elem_wordTable(di, dj);
      
      if( word_len >=  frag_len - 1 ) {
        
        word.push_back( (dj < n_row - 1) ? ',' : '\n' );

        writer.write_fragment( data.get_elem_cumWordTable(di, dj) - frag_len, word.c_str(), frag_len );
        
        word.clear();
        word_len = 0;
        
        if( ++di == n_col ) {
          
          di = 0;
          ++dj;
        }
      }
    }
  }
}

int main() {
  
  // WriteTransposedDataHandler transpose_data("output.csv", "output_transpose.csv", 20708, 2*100*2^20, 100*2^20);
  
  WriteTransposedDataHandler transpose_data("extest.csv", "extest2.csv", 90, 90);
  
  // left off with this working. the chunk writing does not seem to work! Which part is breaking???
  
  // transpose_data.write_transpose();
  
}

// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o WriteTransposedDataHandlerTest src/FileHandler.cpp src/FileWriteHandler.cpp src/MMapHandler.cpp src/MMapWriteHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp src/WriteTransposedDataHandler.cpp -L/usr/local/lib/R/lib -lR














