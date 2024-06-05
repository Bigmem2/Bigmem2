#ifndef MTDATAFRAME_H
#define MTDATAFRAME_H

#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <stdexcept>
#include <sstream>
using namespace Rcpp;

class MTDataFrame {
public:
  using DataType = std::variant<std::string, double, int>;

  // constructor
  MTDataFrame(const int& n);

  void add_row(const std::vector<DataType>& row);

  DataType get_element(size_t row, size_t col) const;

  // void add_column(const std::vector<DataType>& col, const int& pos) {
  //   for(size_t i = 0; i < col.size(); ++i) {
  //     data[pos][i].push_back(col[i]);
  //   }
  // }

  void print() const;

private:
  std::vector<std::vector<DataType>> data;
};

#endif //MTDATAFRAME_H
