#ifndef MTSUBTABLE_H
#define MTSUBTABLE_H

#include <Rcpp.h>
#include "MTDataFrame.h"
using namespace Rcpp;

class MTSubTable {
private:
  int n_cols;
  int n_rows;
  double c_size;
  int xr;
  int yr;
  MTDataFrame df;


public:
  using DataType = std::variant<std::string, double, int>;

  MTSubTable();

  MTSubTable(int n_cols, int n_rows, double c_size, int xr, int yr);

  void set_n_cols(double n_cols);

  void set_n_rows(double n_rows);

  void set_c_size(double c_size);

  void set_xr(double xr);

  void set_yr(double yr);

  void set_df(MTDataFrame df);

  void table_add_row(const std::vector<DataType>&);

  int get_n_cols();

  int get_n_rows();

  double get_c_size();

  int get_xr();

  int get_yr();

  MTDataFrame get_df();

  void serialize(std::ofstream& ofs) const;

  void deserialize(std::ifstream& ifs);

};

#endif //MTSUBTABLE_H
