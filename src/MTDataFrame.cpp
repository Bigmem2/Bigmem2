// #include <Rcpp.h>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <variant>
// #include <stdexcept>
// #include <sstream>
//
// using namespace Rcpp;

#include "MTDataFrame.h"

// RCPP_EXPOSED_CLASS(MTDataFrame);

MTDataFrame::MTDataFrame(const int& n) : data(n) {}

void MTDataFrame::add_row(const std::vector<MTDataFrame::DataType>& row) {
  for(size_t i = 0; i < row.size(); ++i) {
    data[i].push_back(row[i]);
  }
}

MTDataFrame::DataType MTDataFrame::get_element(size_t row, size_t col) const {
  return data[col][row];
}

// RCPP_MODULE(MTDataFrameEx) {
//   Rcpp::class_<MTDataFrame>("MTDataFrame")
//   .constructor<int>()
//   .method("add_row", &MTDataFrame::add_row)
//   .method("get_element", &MTDataFrame::get_element);
// }

// class MTDataFrame {
// public:
//   using DataType = std::variant<std::string, double, int>;
//
//   // constructor
//   MTDataFrame(const int& n) : data(n) {}
//
//   void add_row(const std::vector<DataType>& row) {
//     for(size_t i = 0; i < row.size(); ++i) {
//       data[i].push_back(row[i]);
//     }
//   }
//
//   DataType get_element(size_t row, size_t col) const {
//     return data[col][row];
//   }
//
//   // void add_column(const std::vector<DataType>& col, const int& pos) {
//   //   for(size_t i = 0; i < col.size(); ++i) {
//   //     data[pos][i].push_back(col[i]);
//   //   }
//   // }
//
// private:
//   std::vector<std::vector<DataType>> data;
// };

// int main() {
//
//   MTDataFrame df(3);
//
//   df.add_row({'mike', 3, 2.888});
//   df.add_row({'jackie', 4, 1.888});
//   df.add_row({'gerard', 5, -2.888});
//
//   return 0;
// }
