#include <Rcpp.h>
#include "MTDataFrame.h"
#include "MTSubTable.h"
#include "MTTable.h"

// create a new MTTable object and return the pointer to R
//[[Rcpp::export]]
SEXP create_MTTable() {
  Rcpp::XPtr<MTTable> ptr(new MTTable(), true);
  return ptr;
}

// initialize by calling r_initiate on an MTTable object via the external pointer
//[[Rcpp::export]]
void r_initiateMTTable(SEXP xp, Rcpp::String r_filepath) {
  Rcpp::XPtr<MTTable> ptr(xp);
  ptr->r_initiate(r_filepath);
}

//[[Rcpp::export]]
Rcpp::DataFrame to_r(SEXP xp) {
  Rcpp::XPtr<MTTable> ptr(xp);
  return ptr->to_r();
}

// to delete: binding to test the serialize routines
// //[[Rcpp::export]]
// SEXP create_MTDataFrame(Rcpp::IntegerVector n) {
//   Rcpp::XPtr<MTDataFrame> ptr(new MTDataFrame(n), true);
//   return ptr;
// }
//
// //[[Rcpp::export]]
// void test_serialize_MTDataFrame(SEXP xp, Rcpp::String r_filepath) {
//   Rcpp::XPtr<MTDataFrame> ptr(xp);
//   ptr->r_test_serialize(r_filepath);
// }


