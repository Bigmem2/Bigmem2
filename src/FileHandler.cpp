#include <Rcpp.h>
#include <fcntl.h>  // For open()
#include <unistd.h> // For close()
#include <string>
#include <stdexcept>
#include <iostream>

class FileHandler {
public:
  // Constructor that opens the file
  FileHandler(const std::string& filename) {
    fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
      throw std::runtime_error("Error opening file: " + filename);
    }
  }

  // Destructor that closes the file
  ~FileHandler() {
    if (fd >= 0) {
      close(fd);
    }
  }

  // Method to get the file descriptor
  int get_fd() const {
    return fd;
  }

private:
  int fd; // File descriptor
};

// // Example usage
// int main() {
//   try {
//     FileHandler file("example.txt");
//
//     // Use file.get_fd() to access the file descriptor
//     int fd = file.get_fd();
//     std::cout << "File descriptor: " << fd << std::endl;
//
//     // Perform file operations here
//
//   } catch (const std::exception& e) {
//     std::cerr << e.what() << std::endl;
//   }
//
//   return 0;
// }
