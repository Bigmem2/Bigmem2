library(devtools)
library(Rcpp)

# # Data to be written to CSV
# data <- data.frame(
#   Name = c("John", "Alice", "Bob"),
#   Age = c(25, 30, 35),
#   City = c("New York", "Los Angeles", "Chicago")
# )
#
# # File path
# file_path <- "../example.csv"
#
# # Write data to CSV file
# write.csv(data, file = file_path, row.names = FALSE)

compileAttributes()
load_all()

read_csv('../example.csv')
read_csv_mmap('../example.csv')
