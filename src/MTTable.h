#ifndef MTTABLE_H
#define MTTABLE_H

#include <Rcpp.h>
#include "MTDataFrame.h"
#include "MTSubTable.h"
#include "FileHandler.h"
using namespace Rcpp;


class MTTable {
private:
  std::string filepath;

  std::vector<int> x;
  std::vector<int> y;
  std::vector<std::string> filepath;
  std::string source_filepath;
  std::vector<MTSubTable> loaded_subtables;

public:
  using DataType = std::variant<std::string, double, int>;

  MTTable();

  MTTable(std::vector<int> x, std::vector<int> y, std::vector<std::string> filepath, std::string source_filepath);

  Rcpp::DataFrame r_initiate(Rcpp::String r_filepath);

  // function to read csv given path as only argument
  // get data into MTSubTable and write to bin,
  // save the bin location/name and coordinates into MTTable
  Rcpp::DataFrame CsvToMTBin();

  // int readMTBin();

  // MTDataFrame readMTBinDataFrame(const std::string& filename);

  // wrap to rcpp to r
  void set_source_filepath(Rcpp::String r_filepath);
  std::string get_source_filepath();

  // mapping between MTTable data and file locations/coordinates
  // std::string coord_to_file(int x, int y);

};



#endif //MTTABLE_H
