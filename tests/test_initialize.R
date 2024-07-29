
library(Bigmem2)

# create an MTTable object and get a pointer
mttable = Bigmem2::create_MTTable()

# call the r_initiate method on the object being pointed at
data = Bigmem2::r_initiateMTTable(mttable, "exdata.csv")




rm(list = ls())












