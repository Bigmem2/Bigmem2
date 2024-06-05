#ifndef MTTABLE_H
#define MTTABLE_H

#include <Rcpp.h>
#include "MTDataFrame.h"
#include "MTSubTable.h"
using namespace Rcpp;


class MTTable {
private:
  std::vector<int> x;
  std::vector<int> y;
  std::vector<std::string> filepath;
  std::string source_filepath;

public:
  using DataType = std::variant<std::string, double, int>;

  MTTable(std::vector<int> x, std::vector<int> y, std::vector<std::string> filepath, std::string source_filepath);

  void r_initiate(Rcpp::String r_filepath);

  // function to read csv given path as only argument
  // get data into MTSubTable and write to bin,
  // save the bin location/name and coordinates into MTTable
  int CsvToMTBin();

  // wrap to rcpp to r
  void set_source_filepath(Rcpp::String r_filepath);
  std::string get_source_filepath();

};



#endif //MTTABLE_H
