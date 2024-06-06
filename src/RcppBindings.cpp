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
