
library(MassiveTable)

# create an MTTable object and get a pointer
mttable = MassiveTable::create_MTTable()

# call the r_initiate method on the object being pointed at
data = MassiveTable::r_initiateMTTable(mttable, "exdata.csv")




rm(list = ls())












