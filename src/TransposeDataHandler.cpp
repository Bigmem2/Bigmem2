#include <Rcpp.h>
#include <chrono> // for testing the timing
#include <thread>
#include "TransposeDataHandler.h"

TransposeDataHandler::TransposeDataHandler(const std::string& filename, off_t chunk_size)
  : read_data(filename, chunk_size), word_starts({0}), cumsum_starts({0}) {

  this->chunk_size = chunk_size;
  
  this->n_row = this->nrow();
  this->n_col = this->ncol();
  
  pre_alloc_wordTable();
  
  fill_wordTable();
  
  transpose(&wordTable, &t_wordTable);

  cumsum(&cum_wordTable);
  transpose(&cum_wordTable, &t_cum_wordTable);
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
  
  return n_row;
}
// idea: the byte pointer vector to each chunk is the start + return of this 
// gets you to the right position
std::vector<int> byte_line_processor(std::string* chunkPtr,
                                     int chunk_size) {
  
  int nLines = 0;
  int bytes = 0;
  std::vector<int> indexes;
  
  for (i = 0; i < chunk_size; ++i) {
    
    ++bytes;
    
    if ( (*chunkPtr)[i] == '\n' ) {
      
      indexes.push_back(bytes);
      bytes = 0;
    }
  }
  
  return nLines;
}

int TransposeDataHandler::detect_bytes_per_line(std::vector<threads>* threads,
                                                int num_threads) {
  // fast search through file to find bytes per line
  
  const std::vector<off_t>& all_chunk_ptrs = read_data->get_all_chunk_ptrs();
  
  int n_total_chunks = all_chunk_ptrs.size();
  int work_per_thread = n_total_chunks / num_threads;
  
  std::string* get_chunk(off_t ptr_start);
  
  for (int i = 0; i < num_threads; ++i) {
    // partition indexing
    int start = i * work_per_thread;
    int end = (i == num_threads - 1) ? total_work : start + work_per_thread; 
    
    threads.push_back(std::thread(byte_line_processor));
    
  }
  
  for (auto& th : threads) {
    
    th.join();
  }
  
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
        
        cum_wrd_ct += wrd_ct;
        
        col_position++;
        
        byte_pos++;
        wrd_ct = 0;
        continue;
      }
      
      if( (element == '\n') ) { 
        
        wordTable[col_position][row_position] = wrd_ct + 1;
        cum_wordTable[col_position][row_position] = wrd_ct + 1;
        
        cum_wrd_ct += wrd_ct;
        
        row_position++;
        
        byte_pos++;
        col_position = 0;
        wrd_ct = 0;
        continue;
      }
      
      byte_pos++;
      wrd_ct++;
    }
  }
  
  read_data.reset();
}

void TransposeDataHandler::transpose(std::vector<std::vector<int>>* table,
                                     std::vector<std::vector<int>>* table_out) {
  
  for(size_t i = 0; i < static_cast<size_t>(n_row); ++i) {
    
    for(size_t j = 0; j < static_cast<size_t>(n_col); ++j) {
      
      (*table_out)[i][j] = (*table)[j][i];
    }
  }
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

int TransposeDataHandler::get_elem_cumWordTable(int i, int j) {
  
  const int* row_ptr = t_cum_wordTable[j].data();
  
  return *(row_ptr + i);
}

ReadDataHandler* TransposeDataHandler::get_ReadDataHandler() {
  
  return &read_data;
}

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






