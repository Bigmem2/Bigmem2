#include <Rcpp.h>
#include "TransposeDataHandler.h"

TransposeDataHandler::TransposeDataHandler(const std::string& filename, off_t chunk_size)
  : read_data(filename, chunk_size), word_starts({0}), cumsum_starts({0}),
    chunk_of_data("") {

}


TransposeDataHandler::index_words() {

  off_t read_position = 0;
  // std::string chunk_of_data;

  while( auto ptr = read_data.next_chunk() ) {

    chunk_of_data = *ptr;

    for( size_t i = 0; i < chunk_of_data.size(); ++i) {

      if( chunk_of_data[i] == "," ) {

        word_starts.push_back( read_position + i + 1 );
      }

    }

    read_position = read_data.get_ptrLocation();
  }

}










// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o TransposeDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR











