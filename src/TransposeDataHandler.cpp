#include <Rcpp.h>
#include <chrono> // for testing the timing
#include "TransposeDataHandler.h"

TransposeDataHandler::TransposeDataHandler(const std::string& filename, off_t chunk_size)
  : read_data(filename, chunk_size), word_starts({0}), cumsum_starts({0}) {

  this->chunk_size = chunk_size;
  
  // this->read_data.get_chunk(0);
  // this->read_data.get_chunk(0);
  
  this->n_row = this->nrow();
  this->n_col = this->ncol();
  
  pre_alloc_wordTable();
  
  fill_wordTable();
  
  std::cout << "Contents of wordTable:" << std::endl;
  for (const auto& row : wordTable) {
    for (const int& value : row) {
      std::cout << value << ", ";  // Use tab to separate columns
    }
    std::cout << std::endl;  // New line after each row
  }
  
  transpose(&wordTable, &t_wordTable);

  std::cout << "Contents of t_wordTable:" << std::endl;
  for (const auto& row : t_wordTable) {
    for (const int& value : row) {
      std::cout << value << ", ";  // Use tab to separate columns
    }
    std::cout << std::endl;  // New line after each row
  }

  cumsum(&cum_wordTable);
  transpose(&cum_wordTable, &t_cum_wordTable);

  std::cout << "Contents of cumsum wordTable:" << std::endl;
  for (const auto& row : t_cum_wordTable) {
    for (const int& value : row) {
      std::cout << value << ", ";  // Use tab to separate columns
    }
    std::cout << std::endl;  // New line after each row
  }
  
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

int TransposeDataHandler::get_n_row() {
  
  return n_row;
}

int TransposeDataHandler::get_n_col() {
  
  return n_col;
}

void TransposeDataHandler::pre_alloc_wordTable() {
  
  wordTable.resize(n_col);
  
  for(auto& column : wordTable) {

    column.resize(n_row);
  }
  
  cum_wordTable.resize(n_col);
  
  for(auto& column : cum_wordTable) {
    
    column.resize(n_row);
  }
  
  t_wordTable.resize(n_row);
  
  for(auto& column : t_wordTable) {
    
    column.resize(n_col);
  }
  
  t_cum_wordTable.resize(n_row);
  
  for(auto& column : t_cum_wordTable) {
    
    column.resize(n_col);
  }
  
  // wordStartsTable.resize(n_col);
  // 
  // for(auto& column : wordStartsTable) {
  //   
  //   column.resize(n_row);
  // }
}

void TransposeDataHandler::fill_wordTable() {
  
  std::string* chunkPtr;
  
  int byte_pos = 0;
  int wrd_ct = 0;
  int cum_wrd_ct = 0;
  
  int row_position = 0;
  int col_position = 0;
  
  while( ( chunkPtr = read_data.next_chunk() ) != nullptr ) {
    
    for(size_t i = 0; i < chunkPtr->size(); ++i) {
      
      char element = (*chunkPtr)[i];
      
      if(element == ',') {
        
        wordTable[col_position][row_position] = wrd_ct + 1;
        cum_wordTable[col_position][row_position] = wrd_ct + 1;
        
        // cum_wordTable[col_position][row_position] = byte_pos;
        cum_wrd_ct += wrd_ct;
        // wordStartsTable[col_position][row_position] = byte_pos - wrd_ct;
        
        col_position++;
        
        byte_pos++;
        wrd_ct = 0;
        continue;
      }
      
      if( (element == '\n') ) { //| (row_position == n_row - 1) 
        
        wordTable[col_position][row_position] = wrd_ct + 1;
        cum_wordTable[col_position][row_position] = wrd_ct + 1;
        
        // cum_wordTable[col_position][row_position] = byte_pos;
        cum_wrd_ct += wrd_ct;
        // wordStartsTable[col_position][row_position] = byte_pos - wrd_ct;
        
        row_position++;
        
        byte_pos++;
        col_position = 0;
        wrd_ct = 0;
        continue;
      }
      
      // cum_wrd_ct += wrd_ct;
      byte_pos++;
      wrd_ct++;
    }
  }
  
  read_data.reset();
}

void TransposeDataHandler::transpose(std::vector<std::vector<int>>* table,
                                     std::vector<std::vector<int>>* table_out) {
  
  // size_t table_rows = table->size();
  // size_t table_cols = (*table)[0].size();
  
  // std::vector<std::vector<int>> table_tranp(table_cols, std::vector<int>(table_rows));
  
  for(size_t i = 0; i < static_cast<size_t>(n_row); ++i) {
    
    for(size_t j = 0; j < static_cast<size_t>(n_col); ++j) {
      
      (*table_out)[i][j] = (*table)[j][i];
    }
  }
  
  // *table = std::move(table_tranp);
}

void TransposeDataHandler::cumsum(std::vector<std::vector<int>>* table) {
  
  size_t n_cols = table->size();
  size_t n_rows = (*table)[0].size();
  
  int cum_sum = 0;
  
  std::vector<std::vector<int>> table_cumsum(n_cols, std::vector<int>(n_rows));
  
  for(size_t j = 0; j < static_cast<size_t>(n_col); ++j) {
    
    for(size_t i = 0; i < static_cast<size_t>(n_row); ++i) {
      
      cum_sum += (*table)[j][i];
      
      table_cumsum[j][i] = cum_sum;
    }
  }
  
  *table = std::move(table_cumsum);
}

int TransposeDataHandler::get_elem_wordTable(int i, int j) {
  
  const int* row_ptr = t_wordTable[j].data();
  
  return *(row_ptr + i);
}

// int TransposeDataHandler::get_elem_wordStartsTable(int i, int j) {
//   
//   const int* row_ptr = wordStartsTable[j].data();
//   
//   return *(row_ptr + i);
// }

int TransposeDataHandler::get_elem_cumWordTable(int i, int j) {
  
  const int* row_ptr = t_cum_wordTable[j].data();
  
  return *(row_ptr + i);
}

ReadDataHandler* TransposeDataHandler::get_ReadDataHandler() {
  
  return &read_data;
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

// int main() {
//   try {
// 
//     auto start = std::chrono::high_resolution_clock::now();
// //3650722816/50
//     TransposeDataHandler data("exdata.csv", 100*2^20);
// 
//     std::cout << "test word table element access " << data.get_elem_wordTable(3, 0) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordTable(3, 1) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordTable(3, 2) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordTable(3, 3) << std::endl;
// 
//     std::cout << "test word table element access " << data.get_elem_wordStartsTable(3, 0) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordStartsTable(3, 1) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordStartsTable(3, 2) << std::endl;
//     std::cout << "test word table element access " << data.get_elem_wordStartsTable(3, 3) << std::endl;
// 
// 
//     auto end = std::chrono::high_resolution_clock::now();
// 
//     auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
// 
//     std::cout << "Elapsed time in milliseconds: " << elapsedMilliseconds << " ms\n";
//     std::cout << "Elapsed time in seconds: " << elapsedSeconds << " s\n";
// 
//   } catch(const std::exception& e) {
// 
//     std::cerr << e.what() << std::endl;
//   }
// }








// new, working
// on linux rstudio server container:
// g++ -O3 -march=native -mtune=native -ffast-math -flto -funroll-loops -fomit-frame-pointer -std=gnu++17 -I"/usr/local/lib/R/include" -DNDEBUG -I'/usr/local/lib/R/site-library/Rcpp/include' -I/usr/lib/llvm-10/include -fopenmp -I./src -fPIC -Wall -g -O2 -o TransponseDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/usr/local/lib/R/lib -lR


















// old, not working
// compile on the fly for testing just this component
// clang++ -arch arm64 -std=gnu++17 -I"/Library/Frameworks/R.framework/Resources/include" -DNDEBUG -I'/Library/Frameworks/R.framework/Versions/4.4-arm64/Resources/library/Rcpp/include' -I/opt/R/arm64/include -I/opt/homebrew/opt/llvm/include -Xclang -fopenmp -I./src -fPIC -falign-functions=64 -Wall -g -O2 -o TransposeDataHandlerTest src/FileHandler.cpp src/MMapHandler.cpp src/ReadDataHandler.cpp src/TransposeDataHandler.cpp -L/Library/Frameworks/R.framework/Resources/lib -lR






