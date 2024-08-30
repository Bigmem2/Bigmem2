#include <Rcpp.h>
#include <string>
#include "MMapWriteHandler.h"
#include "TransposeDataHandler.h"

WriteTransposedDataHandler::WriteTransposedDataHandler(const std::string& filename, 
            off_t read_size, size_t fragment_size, size_t sync_size)
  : writer(filename, size), source_data(filename, read_size) {
  
}

void WriteTransposedDataHandler::write_transpose() {
  
  writer.write_fragment(pos, chunk, )
}

int main() {
  
  WriteTransposedDataHandler writer("exdata2.csv", 100, 2, 10);
  
}

// compile on linux:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o WriteTransposedDataHandlerTest src/FileWriteHandler.cpp src/MMapWriteHandler.cpp src/WriteTransposedDataHandler.cpp -L/usr/local/lib/R/lib -lR
