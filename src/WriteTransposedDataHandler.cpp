#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"
#include "WriteTransposedDataHandler.h"

WriteTransposedDataHandler::WriteTransposedDataHandler(const std::string& filename, 
  off_t size, size_t sync_threshold, size_t read_size)
  : writer(filename, size, sync_threshold), data(filename, read_size) {
  
  // write_transpose();
}

void WriteTransposedDataHandler::write_transpose() {
  
  ReadDataHandler* read_data = data.get_ReadDataHandler();
  
  std::string* chunkPtr;
  int di = 0;
  int dj = 0;
  
  while( ( chunkPtr = read_data->next_chunk()) != nullptr ) {
    
    std::string word;
    int word_len = 0;
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      // do ...
      char element = (*chunkPtr)[i];
      
      word.push_back( element );
      word_len += 1;
      
      frag_len = data.get_elem_wordTable(di, dj);
      
      if( word_len >=  frag_len ) {

        writer.write_fragment( data.get_elem_wordStartsTable(di, dj), word.c_str(), frag_len );
        word.clear();
        
        di += 1;
        
        if( di >= ( data.get_n_row() - 1 ) ) {
          
          dj += 1;
        }
      }
      
      
      // char element = (*chunkPtr)[i];
      // 
      // if(element == '\n') {
      //   
      //   n_row++;
      // }
    }
  }
  
  
  
}

int main() {
  
  WriteTransposedDataHandler transpose_data("exdata2.csv", 101, 101, 101);
  
  transpose_data.write_transpose();
  
}

// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o WriteTransposedDataHandlerTest src/FileWriteHandler.cpp src/MMapWriteHandler.cpp src/WriteTransposedDataHandler.cpp -L/usr/local/lib/R/lib -lR















