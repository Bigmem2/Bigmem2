// #include <Rcpp.h>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <variant>
// #include <stdexcept>
// #include <sstream>
//
// using namespace Rcpp;

#include <Rcpp.h>
#include <iostream>
#include <fstream>
#include "MTDataFrame.h"
using namespace Rcpp;

// RCPP_EXPOSED_CLASS(MTDataFrame);

MTDataFrame::MTDataFrame() : data(0) {}

MTDataFrame::MTDataFrame(const Rcpp::IntegerVector n) : data(n(0)) {}

MTDataFrame::MTDataFrame(const int& n) : data(n) {}

void MTDataFrame::add_row(const std::vector<MTDataFrame::DataType>& row) {
  for(size_t i = 0; i < row.size(); ++i) {
    data[i].push_back(row[i]);
  }
}

MTDataFrame::DataType MTDataFrame::get_element(size_t row, size_t col) const {
  return data[col][row];
}

Rcpp::DataFrame MTDataFrame::to_r() {

  Rcpp::DataFrame df;

  size_t nCols = data.size();

  std::vector<std::vector<std::string>> stringCols(nCols);
  std::vector<std::vector<double>> doubleCols(nCols);
  std::vector<std::vector<int>> intCols(nCols);



  for (size_t i = 0; i < data.size(); ++i) {

    df.push_back(data[i]);
  }

  return df;
}

void MTDataFrame::print() const {
  // // Print column headers
  // for (const auto& col : columns) {
  //   Rcout << col << "\t";
  // }
  // Rcout << std::endl;

  // Print rows
  if (!data.empty()) {
    size_t numRows = data[0].size();
    for (size_t i = 0; i < numRows; ++i) {
      for (size_t j = 0; j < data.size(); ++j) {
        std::visit([](const auto& value) { Rcout << value << "\t"; }, data[j][i]);
      }
      Rcout << std::endl;
    }
  }

}

void MTDataFrame::serialize(std::ofstream& ofs) const {

  size_t outerSize = data.size();
  ofs.write(reinterpret_cast<const char*>(&outerSize), sizeof(outerSize));

  for (const auto& innerVec : data) {

    size_t innerSize = innerVec.size();
    ofs.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));

    for (const auto& variant : innerVec) {

      size_t index = variant.index();
      ofs.write(reinterpret_cast<const char*>(&index), sizeof(index));

      std::visit([&ofs](const auto& value) {

        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::string>) {
          size_t length = value.size();
          ofs.write(reinterpret_cast<const char*>(&length), sizeof(length));
          ofs.write(value.c_str(), length);
        } else {

          ofs.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
      }, variant);
    }
  }
}

void MTDataFrame::deserialize(std::ifstream& ifs) {

  size_t outerSize;
  ifs.read(reinterpret_cast<char*>(&outerSize), sizeof(outerSize));

  data.resize(outerSize);

  for (size_t i = 0; i < outerSize; ++i) {

    size_t innerSize;
    ifs.read(reinterpret_cast<char*>(&innerSize), sizeof(innerSize));

    data[i].resize(innerSize);

    for (size_t j = 0; j < innerSize; ++j) {

      size_t index;
      ifs.read(reinterpret_cast<char*>(&index), sizeof(index));

      switch (index) {
      case 0: { // std::string
        size_t length;
        ifs.read(reinterpret_cast<char*>(&length), sizeof(length));
        std::string value(length, ' ');
        ifs.read(&value[0], length);
        data[i][j] = value;
        break;
      }
      case 1: { // double
        double value;
        ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
        data[i][j] = value;
        break;
      }
      case 2: { // int
        int value;
        ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
        data[i][j] = value;
        break;
      }
      default:
        throw std::runtime_error("Unknown variant index");
      }
    }
  }
}

// void MTDataFrame::r_test_serialize(Rcpp::String r_filepath) {
//
//   MTDataFrame df(2);
//   df.add_row({1,2});
//
// }










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
