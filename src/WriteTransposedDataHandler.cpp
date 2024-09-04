#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"
#include "WriteTransposedDataHandler.h"

WriteTransposedDataHandler::WriteTransposedDataHandler(const std::string& filename, 
  const std::string& filename2, off_t size, size_t sync_threshold, size_t read_size)
  : writer(filename2, size, sync_threshold), data(filename, read_size) {
  
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
    int frag_len = 0;
    int offset = 0;

    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      // do ...
      char element = (*chunkPtr)[i];
      
      if( element == ','  || element == '\n' ) {
        continue;
      }
      
      word.push_back( element );
      word_len += 1;
      
      frag_len = data.get_elem_wordTable(di, dj);
      std::cout << "di " << di << "\n" << "dj " << dj << std::endl;
      std::cout << "frag len " << frag_len << std::endl;
      
      if( word_len >=  frag_len ) {
        
        std::cout << "word " << word << std::endl;
        
        
        
        if( dj < data.get_n_col() - 1 ) { 
          
          word.push_back( ',' );
        } else {
          
          word.push_back( '\n' );
        }
        // frag_len += 1;
        // word_len += 1;
        // offset = 1;
        
        std::cout << "word " << word << std::endl;

        // writer.write_fragment( data.get_elem_wordStartsTable(di, dj) + cum_frag_len, word.c_str(), frag_len );
        writer.write_fragment( data.get_elem_cumWordTable(di, dj) - word_len + 1, word.c_str(), frag_len );
        word.clear();
        
        di += 1;
        
        std::cout << "data.get_n_row() " << data.get_n_row() << std::endl;
        if( di == data.get_n_row() ) {
          
          di = 0;
          dj += 1;
        }
        
        offset += 1;
        
        word_len = 0;
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
  
  WriteTransposedDataHandler transpose_data("exdata.csv", "exdata2.csv", 101, 101, 101);
  
  transpose_data.write_transpose();
  
}

// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o WriteTransposedDataHandlerTest src/FileHandler.cpp src/FileWriteHandler.cpp src/MMapHandler.cpp src/MMapWriteHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp src/WriteTransposedDataHandler.cpp -L/usr/local/lib/R/lib -lR














