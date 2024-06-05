
test_install = function(load = FALSE) {

  clean_dll()
  compileAttributes()

  if(load) {
    load_all()
  } else {
    install(dependencies = FALSE)
  }

}
