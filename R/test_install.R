
test_install = function(load = FALSE) {

  devtools::clean_dll()
  Rcpp::compileAttributes()

  if(load) {
    devtools::load_all()
  } else {
    devtools::install(dependencies = FALSE)
  }

}
