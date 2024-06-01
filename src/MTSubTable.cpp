#include <Rcpp.h>
using namespace Rcpp;

class MTSubTable {
private:
  int n_cols;
  int n_rows;
  double c_size;
  int xr;
  int yr;

public:
  MTSubTable(int n_cols, int n_rows, double c_size, int xr, int yr) {
    this->n_cols = n_cols;
    this->n_rows = n_rows;
    this->c_size = c_size;
    this->xr = xr;
    this->yr = yr;
    // add stuff of the data table object, need a data table class!
  }

  void set_n_cols(double n_cols) {
    this->n_cols = n_cols;
  }

  void set_n_rows(double n_rows) {
    this->n_rows = n_rows;
  }

  void set_c_size(double c_size) {
    this->c_size = c_size;
  }

  void set_xr(double xr) {
    this->xr = xr;
  }

  void set_yr(double yr) {
    this->yr = yr;
  }

  int get_n_cols() {
    return n_cols;
  }

  int get_n_rows() {
    return n_rows;
  }

  double get_c_size() {
    return c_size;
  }

  int get_xr() {
    return xr;
  }

  int get_yr() {
    return yr;
  }

};

RCPP_MODULE(MTSubTableEx) {
  class_<MTSubTable>("MTSubTable")
  .constructor<int,int,double,int,int>()
  .method("set_n_cols", &MTSubTable::set_n_cols)
  .method("getX", &MTSubTable::getX)
  .method("square", &MTSubTable::square);
}
