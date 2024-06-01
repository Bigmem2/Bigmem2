library(devtools)
library(Rcpp)

clean_dll()
compileAttributes()
# document()
# load_all()
install()

library(MassiveTable)

obj = new(MyClass, 10)

obj$setX(20)
obj$getX()
obj$square()
